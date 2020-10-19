#include <iostream>
#include <random>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#define WIDTH 500
#define HEIGHT 500
#define COUNT 2
#define CP 8 //큐브의 점 수
#define PP 5 //사각뿔의 점 수
#define SIZE 0.1


GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid ArrowKey(int, int, int);

GLint select = 0, dx = 1, dy = 1, one = 0, t = 100;
GLboolean w = false, h = false, tx = false, ty = false, stop = true;
GLfloat cube_pos[CP][3] = {
	-SIZE,-SIZE,SIZE,
	SIZE,-SIZE,SIZE,
	SIZE,-SIZE,-SIZE,
	-SIZE,-SIZE,-SIZE,
	-SIZE,SIZE,SIZE,
	SIZE,SIZE,SIZE,
	SIZE,SIZE,-SIZE,
	-SIZE,SIZE,-SIZE
};
GLfloat corigin_pos[CP][3] = {
	-SIZE,-SIZE,SIZE,
	SIZE,-SIZE,SIZE,
	SIZE,-SIZE,-SIZE,
	-SIZE,-SIZE,-SIZE,
	-SIZE,SIZE,SIZE,
	SIZE,SIZE,SIZE,
	SIZE,SIZE,-SIZE,
	-SIZE,SIZE,-SIZE
};
GLfloat pyramid_pos[PP][3] = {
	-SIZE,-SIZE,SIZE,
	SIZE,-SIZE,SIZE,
	SIZE,-SIZE,-SIZE,
	-SIZE,-SIZE,-SIZE,
	0.0,SIZE,0.0
};
GLfloat cpyramid_pos[PP][3] = {
	-SIZE,-SIZE,SIZE,
	SIZE,-SIZE,SIZE,
	SIZE,-SIZE,-SIZE,
	-SIZE,-SIZE,-SIZE,
	0.0,SIZE,0.0
};
GLint cube_index[36] = {
	0,1,5,
	0,5,4,
	1,2,6,
	1,6,5,
	2,3,6,
	3,7,6,
	0,7,3,
	0,4,7,
	4,5,6,
	4,6,7,
	0,2,1,
	0,3,2,
};
GLint pyramid_index[18] = {
	0,1,4,
	0,2,4,
	2,3,4,
	0,4,3,
	0,3,2,
	0,2,1
};

GLfloat grid_pos[4][3] = {
	-1.0,0.0,0.0,
	1.0,0.0,0.0,
	0.0,-1.0,0.0,
	0.0,1.0,0.0
};

GLfloat grid_color[4][3] = { 0.0f, };
GLfloat cube_color[CP][3];
GLfloat pyramid_color[PP][3];

glm::mat4 R = glm::mat4(1.0f);
glm::mat4 T = glm::mat4(1.0f);
glm::mat4 Rx = glm::mat4(1.0f);
glm::mat4 Ry = glm::mat4(1.0f);
glm::mat4 Tx = glm::mat4(1.0f);
glm::mat4 Ty = glm::mat4(1.0f);
unsigned int modelLocation;

float randomRGB()
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> uid(0, 255);
	// 255 : uid = 1.0f : x , x = uid / 255 
	return 1.0f * uid(mt) / 255;
}


char* filetobuf(const char *file)
{
	FILE *fptr;
	long length;
	char *buf;

	fopen_s(&fptr, file, "rb");
	if (!fptr) return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}

GLchar *vertexsource, *fragmentsource;
GLuint vertexshader, fragmentshader;
GLuint vao[COUNT], vbo[COUNT][2], ebo[COUNT];
GLuint s_program;

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");

	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

	glCompileShader(vertexshader);

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR : vertex shader 컴파일실패\n" << errorLog << endl;

	}
}
void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");

	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

	glCompileShader(fragmentshader);

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR : vertex shader 컴파일실패\n" << errorLog << endl;

	}
}



void InitShader()
{
	make_vertexShader();
	make_fragmentShader();

	s_program = glCreateProgram();

	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");

	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	glUseProgram(s_program);
}

void InitBuffer()
{
	for (int i = 0; i < CP; i++)
		for (int j = 0; j < 3; j++)cube_color[i][j] = randomRGB();
	for (int i = 0; i < PP; i++)
		for (int j = 0; j < 3; j++)pyramid_color[i][j] = randomRGB();

	for (int i = 0; i < COUNT; i++)
	{
		glGenVertexArrays(1, &vao[i]);

		glBindVertexArray(vao[i]);
		glGenBuffers(2, vbo[i]);

		//위치
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);
		//if(i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(grid_pos), grid_pos, GL_STATIC_DRAW);
		if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(cube_pos), cube_pos, GL_STATIC_DRAW);
		else if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_pos), pyramid_pos, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		//색상
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);

		//if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(grid_color), grid_color, GL_STATIC_DRAW);
		if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);
		if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_color), pyramid_color, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);

		//EBO 설정

		glGenBuffers(1, &ebo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		if (i == 0) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index), cube_index, GL_STATIC_DRAW);
		if (i == 1) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_index), pyramid_index, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);


	}

}


void UpdateBuffer()
{
	for (int i = 0; i < COUNT; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);


		if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(corigin_pos), corigin_pos, GL_STATIC_DRAW);
		else if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(cpyramid_pos), cpyramid_pos, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);
	}
}

void InitRotate()
{
	R = glm::mat4(1.0f);

	Rx = glm::mat4(1.0f);
	Ry = glm::mat4(1.0f);

	glUseProgram(s_program);
	modelLocation = glGetUniformLocation(s_program, "modelTransform");

	Rx = glm::rotate(Rx, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	Ry = glm::rotate(Ry, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));

	R = Rx * Ry;

	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R));
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(WIDTH, HEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Example"); // 윈도우 생성(윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	
	InitShader();
	InitBuffer();
	InitRotate();



	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard);
	glutSpecialFunc(ArrowKey);
	glutMainLoop(); // 이벤트 처리 시작 
}

void DrawCL()
{
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R));

	glBegin(GL_LINES);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glEnd();
}


GLvoid ArrowKey(int key, int x, int y)
{

	if (key == GLUT_KEY_DOWN)
	{
		Ty = glm::translate(Ty, glm::vec3(0.0, -0.5, 0.0));
	}
	if (key == GLUT_KEY_UP)
	{
		Ty = glm::translate(Ty, glm::vec3(0.0, 0.5, 0.0));

	}
	if (key == GLUT_KEY_LEFT)
	{
		Tx = glm::translate(Tx, glm::vec3(-0.5, 0.0, 0.0));
	}
	if (key == GLUT_KEY_RIGHT)
	{
		Tx = glm::translate(Tx, glm::vec3(0.5, 0.0, 0.0));
	}

	T = Tx * Ty;

	glutPostRedisplay();
}

GLvoid KeyBoard(unsigned char key, int x, int y)
{

	if (key == 'c') select = 0;
	if (key == 'p') select = 1;

	if (key == 'w') w = true;
	if (key == 'W') w = false;

	if (key == 'h') {
		if (!h)
		{
			h = true;
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
		}
		else
		{
			h = false;
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		}
	}

	if (key == 'x' || key == 'x' || key == 'y' || key == 'y')
	{
		if (stop == true) glutTimerFunc(t, TimerFunction, 1);

		if (key == 'x' || key == 'x')
		{

			stop = false;
			if (key == 'x') dx = 1;
			else dx = -1;
			tx = true;
			ty = false;

		}

		if (key == 'y' || key == 'y')
		{

			stop = false;
			if (key == 'y') dy = 1;
			else dy = -1;
			ty = true;
			tx = false;

		}
	}
	if (key == 's' || key == 'S')
	{
		Tx = glm::mat4(1.0f);
		Ty = glm::mat4(1.0f);
		T = glm::mat4(1.0f);
		InitRotate();
		stop = true;
	}
	glutPostRedisplay();
}

void Rotate()
{
	if (tx)
	{
		Rx = glm::rotate(Rx, glm::radians(dx * 30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		R = Rx * Ry;
	}

	if (ty)
	{
		Ry = glm::rotate(Ry, glm::radians(dy * 30.0f), glm::vec3(0.0, 1.0, 0.0));
		R = Rx * Ry;
	}

}

void init_Trans(int i)
{
	Tx = glm::mat4(1.0f);
	if (i == 0)
	{
		Tx = glm::translate(Tx, glm::vec3(0.5, 0.0, 0.0));
	}
	if (i == 1)
	{
		Tx = glm::translate(Tx, glm::vec3(-0.5, 0.0, 0.0));
	}
	T = Tx * Ty;
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (stop) return;

	Rotate();
	glutPostRedisplay();
	glutTimerFunc(t, TimerFunction, 1);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	glUseProgram(s_program);

	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	DrawCL();

	init_Trans(0);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R*T));
	glBindVertexArray(vao[0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	init_Trans(1);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R*T));
	glBindVertexArray(vao[1]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glutSwapBuffers(); // 화면에 출력하기

}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}