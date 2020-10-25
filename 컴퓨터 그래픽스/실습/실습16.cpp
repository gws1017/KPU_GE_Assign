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
#define COUNT 8 //0 큐브(앞,윗면 없음) 1 피라미드 2(큐브 윗면) 3(큐브 앞면) 4567(피라미드 옆면)
#define CP 8 //큐브의 점 수
#define PP 5 //사각뿔의 점 수
#define NC 34 //두면을 뺀 큐브의 인덱스 갯수
#define CFP 4 //CUBE FACE POINT
#define PFP 3 //PYRAMID FACE POINT
#define SIZE 0.3

glm::mat4 projection = glm::mat4(1.0f);

glm::mat4 view = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraPos2 = glm::vec3(0.0f, 1.0f, 8.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid ArrowKey(int, int, int);

GLint select = 0, dx = 1, dy = 0, one = 0, t = 100, up = 0, fangle = 0, df = 0, df2 = 0,fangle2 = 0;
GLboolean w = false, h = true,stop = true, open = false, open2 = false, p = true;

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
GLfloat cuf_pos[CFP][3] ={
	-SIZE,0.0f,-SIZE, //0
	-SIZE,0.0f,SIZE,  //1
	SIZE,0.0f,SIZE,   //2
	SIZE,0.0f,-SIZE  //3
};
GLfloat cff_pos[CFP][3] = {
	-SIZE,SIZE,0.0f, //0
	-SIZE,-SIZE,0.0f,  //1
	SIZE,-SIZE,0.0f,   //2
	SIZE,SIZE,0.0f  //3
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
GLfloat pff_pos[PFP][3] = {
	-SIZE,-SIZE,SIZE, //0
	SIZE,-SIZE,SIZE,  //1
	0.0,SIZE,0.0,   //2
};
GLfloat plf_pos[PFP][3] = {
	-SIZE,-SIZE,-SIZE, //0
	-SIZE,-SIZE,SIZE,  //1
	0.0,SIZE,0.0,   //2
};
GLfloat prf_pos[PFP][3] = {
	SIZE,-SIZE,-SIZE, //0
	SIZE,-SIZE,SIZE,  //1
	0.0,SIZE,0.0   //2
};
GLfloat pbf_pos[PFP][3] = {
	SIZE,-SIZE,-SIZE, //0
	-SIZE,-SIZE,-SIZE,  //1
	0.0,SIZE,0.0   //2
};
GLfloat cpyramid_pos[PP][3] = {
	-SIZE,-SIZE,SIZE,
	SIZE,-SIZE,SIZE,
	SIZE,-SIZE,-SIZE,
	-SIZE,-SIZE,-SIZE,
	0.0,SIZE,0.0
};


GLint cube_index[NC] = {
	1,2,6,
	1,6,5,
	2,3,6,
	3,7,6,
	0,7,3,
	0,4,7,
	0,2,1,
	0,3,2,
};
GLint cube_f_index[6] = {
	0,1,2,
	0,2,3	
};



GLint pyramid_index[6] = {
	//0,1,4, //정면
	//1,2,4, //오른면
	//2,3,4, // 뒷면
	//0,4,3, // 왼면
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

GLfloat cube_color[CP][3] = {
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f
};
GLfloat cuf_color[CFP][3] = {
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
};
GLfloat cff_color[CFP][3] = {
	0.0f,1.0f,1.0f,
	0.0f,1.0f,1.0f,
	0.0f,1.0f,1.0f,
	0.0f,1.0f,1.0f,

};

GLfloat pyramid_color[PP][3] = {
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
	1.0f,0.0f,1.0f,
};
GLfloat pff_color[PFP][3] = {
	0.0f,0.0f,1.0f,
};
GLfloat plf_color[PFP][3] = {
	1.0f,0.0f,1.0f,
};
GLfloat prf_color[PFP][3] = {
	0.0f,1.0f,1.0f,
};
GLfloat pbf_color[PFP][3] = {
	1.0f,1.0f,0.0f,
};

glm::mat4 cl = glm::mat4(1.0f);
glm::mat4 SR = glm::mat4(1.0f);

glm::mat4 UF = glm::mat4(1.0f);
glm::mat4 UFR = glm::mat4(1.0f);
glm::mat4 UFT = glm::mat4(1.0f);

glm::mat4 FF = glm::mat4(1.0f);
glm::mat4 FFT = glm::mat4(1.0f);
glm::mat4 FFTY = glm::mat4(1.0f);
glm::mat4 FFR = glm::mat4(1.0f);

glm::mat4 PF = glm::mat4(1.0f);
glm::mat4 PFT = glm::mat4(1.0f);
glm::mat4 PFTY = glm::mat4(1.0f);
glm::mat4 PFR = glm::mat4(1.0f);

glm::mat4 PF2 = glm::mat4(1.0f);
glm::mat4 PFT2 = glm::mat4(1.0f);
glm::mat4 PFTY2 = glm::mat4(1.0f);
glm::mat4 PFR2 = glm::mat4(1.0f);

glm::mat4 PF3 = glm::mat4(1.0f);
glm::mat4 PFT3 = glm::mat4(1.0f);
glm::mat4 PFTY3 = glm::mat4(1.0f);
glm::mat4 PFR3 = glm::mat4(1.0f);

glm::mat4 PF4 = glm::mat4(1.0f);
glm::mat4 PFT4 = glm::mat4(1.0f);
glm::mat4 PFTY4 = glm::mat4(1.0f);
glm::mat4 PFR4 = glm::mat4(1.0f);

glm::mat4 R = glm::mat4(1.0f);
glm::mat4 T = glm::mat4(1.0f);

glm::mat4 Rx = glm::mat4(1.0f);
glm::mat4 Ry = glm::mat4(1.0f);

glm::mat4 Tx = glm::mat4(1.0f);
glm::mat4 Ty = glm::mat4(1.0f);
unsigned int modelLocation,projectionLocation, viewLocation;

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

	for (int i = 0; i < COUNT; i++)
	{
		glGenVertexArrays(1, &vao[i]);
		glBindVertexArray(vao[i]);
		glGenBuffers(2, vbo[i]);

		//위치
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);
		if (i == 0 ) glBufferData(GL_ARRAY_BUFFER, sizeof(cube_pos), cube_pos, GL_STATIC_DRAW);
		else if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_pos), pyramid_pos, GL_STATIC_DRAW);
		else if (i == 2) glBufferData(GL_ARRAY_BUFFER, sizeof(cuf_pos), cuf_pos, GL_STATIC_DRAW);
		else if (i == 3) glBufferData(GL_ARRAY_BUFFER, sizeof(cff_pos), cff_pos, GL_STATIC_DRAW);
		else if (i == 4) glBufferData(GL_ARRAY_BUFFER, sizeof(pff_pos), pff_pos, GL_STATIC_DRAW);
		else if (i == 5) glBufferData(GL_ARRAY_BUFFER, sizeof(plf_pos), plf_pos, GL_STATIC_DRAW);
		else if (i == 6) glBufferData(GL_ARRAY_BUFFER, sizeof(prf_pos), prf_pos, GL_STATIC_DRAW);
		else if (i == 7) glBufferData(GL_ARRAY_BUFFER, sizeof(pbf_pos), pbf_pos, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//색상
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);
		if (i == 0 ) glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);
		else if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_color), pyramid_color, GL_STATIC_DRAW);
		else if (i == 2) glBufferData(GL_ARRAY_BUFFER, sizeof(cuf_color), cuf_color, GL_STATIC_DRAW);
		else if (i == 3) glBufferData(GL_ARRAY_BUFFER, sizeof(cff_color), cff_color, GL_STATIC_DRAW);
		else if (i == 4) glBufferData(GL_ARRAY_BUFFER, sizeof(pff_color), pff_color, GL_STATIC_DRAW);
		else if (i == 5) glBufferData(GL_ARRAY_BUFFER, sizeof(plf_color), plf_color, GL_STATIC_DRAW);
		else if (i == 6) glBufferData(GL_ARRAY_BUFFER, sizeof(prf_color), prf_color, GL_STATIC_DRAW);
		else if (i == 7) glBufferData(GL_ARRAY_BUFFER, sizeof(pbf_color), pbf_color, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		//EBO 설정

		glGenBuffers(1, &ebo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		if (i == 0) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index), cube_index, GL_STATIC_DRAW);
		if (i == 1) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_index), pyramid_index, GL_STATIC_DRAW);
		if (i == 2 || i == 3) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_f_index), cube_f_index, GL_STATIC_DRAW);
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

void InitRT()
{
	R = glm::mat4(1.0f);

	Rx = glm::mat4(1.0f);
	Ry = glm::mat4(1.0f);

	glUseProgram(s_program);
	modelLocation = glGetUniformLocation(s_program, "modelTransform");

	Rx = glm::rotate(Rx, glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0));
	Ry = glm::rotate(Ry, glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));

	R = Rx * Ry;
	cl = R;
	Ry = glm::mat4(1.0f);

	UFT = glm::translate(UFT, glm::vec3(0.0f, 0.3f, 0.0f));
	FFT = glm::translate(FFT, glm::vec3(0.0f, 0.0f, 0.3f));
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
	InitRT();



	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard);
	glutSpecialFunc(ArrowKey);
	glutMainLoop(); // 이벤트 처리 시작 
}

void DrawCL()
{
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cl));

	glBegin(GL_LINES);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f,1.0f);
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

	if (key == '1') select = 0;
	if (key == '2') select = 1;

	if (key == 'p') p = false;
	if (key == 'P') p = true;

	if (key == 'w') w = true;
	if (key == 'W') w = false;

	if (key == 'h') {
		if (!h)
		{
			h = true;
			
			
		}
		else
		{
			h = false;
			
			
		}
	}

	if (key == 't' || key == 'T' || key == 'y' || key == 'f' || key == 'F' || key == 'o'|| key == 'O')
	{
		if (stop == true)
		{
			stop = false;
			glutTimerFunc(t, TimerFunction, 1);
		}

		if (key == 't')
		{
			up = 1;
		}

		if (key == 'T')
		{
			up = 0;
		}

		if (key == 'f')
		{
			open = true;
			df = 1;
		}
		if (key == 'F')
		{
			open = false;
			df = -1;
		}
		if (key == 'o')
		{
			open2 = true;
			df2 = 1;
		}
		if (key == 'O')
		{
			open = false;
			df2 = -1;
		}
		if (key == 'y')
		{
			if (dy == 0) dy = 1;
			else if(dy == 1) dy = 0;
		}
		
	}
	if (key == 's' || key == 'S')
	{
		Tx = glm::mat4(1.0f);
		Ty = glm::mat4(1.0f);
		T = glm::mat4(1.0f);
		InitRT();
		stop = true;
	}
	
	glutPostRedisplay();
}

void Rotate()
{
	UFR = glm::rotate(UFR, glm::radians(up * 30.0f), glm::vec3(1.0f,0.0f,0.0f));

	FFR = glm::rotate(FFR, glm::radians(df * 10.0f), glm::vec3(1.0f, 0.0f,0.0f));

	PFR = glm::rotate(PFR, glm::radians(df2 * 10.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	PFR2 = glm::rotate(PFR2, glm::radians(df2 * 10.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	PFR3 = glm::rotate(PFR3, glm::radians(df2 * -10.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	PFR4 = glm::rotate(PFR4, glm::radians(df2 * -10.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	Ry = glm::rotate(Ry, glm::radians(dy * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	SR = Ry;
	
}
void Translate()
{
	FFTY = glm::translate(FFTY, glm::vec3(0.0f, df*-0.03f, df*0.03f));

	PFTY = glm::translate(PFTY, glm::vec3(0.0f, 0.0f, df2*0.066f));

	PFTY2 = glm::translate(PFTY2, glm::vec3(df2*-0.066f, 0.0f, 0.0f));

	PFTY3 = glm::translate(PFTY3, glm::vec3(df2*0.066f, 0.0f, 0.0f));

	PFTY4 = glm::translate(PFTY4, glm::vec3(0.0f, 0.0f, df2*-0.066f));
}
GLvoid TimerFunction(int value)
{
	if (stop) return;
	
	if(df != 0) fangle++;
	if (df2 != 0) fangle2++;

	if (fangle % 10 == 0)
	{
		df = 0;
		fangle = 0;
	}

	if (fangle2 % 10 == 0)
	{
		df2 = 0;
		fangle2 = 0;
	}

	Rotate();
	Translate();
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

	if(p == true)view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	else view = glm::lookAt(cameraPos2, cameraDirection, cameraUp);
	viewLocation = glGetUniformLocation(s_program, "viewTransform");

	if(p == true)projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 50.0f);
	else projection = glm::ortho(-2.0f,2.0f,-2.0f,2.0f,0.1f,50.0f);
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	if (!w)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if(h)glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R*T));

	DrawCL();
	
	UF = UFT * UFR;

	FF = FFTY * FFT * FFR;

	PF = PFTY * PFR;
	PF2 = PFTY2 * PFR2;
	PF3 = PFTY3 * PFR3;
	PF4 = PFTY4 * PFR4;

	glBindVertexArray(vao[select]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R * SR));
	
	if (select == 0)
	{
		glDrawElements(GL_TRIANGLES, NC, GL_UNSIGNED_INT, 0);

		glBindVertexArray(vao[2]);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R * SR * UF));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(vao[3]);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R * SR * FF));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	if (select == 1)
	{
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

		
		for (int i = 4; i < COUNT; i++)
		{
			glBindVertexArray(vao[i]);
			if(i == 4)glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R * SR * PF));
			if (i == 5)glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R * SR * PF2));
			if (i == 6)glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R * SR * PF3));
			if (i == 7)glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R * SR * PF4));
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
			
			
		
	}

	glutSwapBuffers(); // 화면에 출력하기

}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}