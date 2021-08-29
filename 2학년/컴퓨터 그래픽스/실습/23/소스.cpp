#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <random>
#include "ReadObj.h"
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"

using namespace std;

#define WIDTH 500
#define HEIGHT 500
#define COUNT 2
#define CP 8 //큐브의 점 수
#define PP 5 //사각뿔의 점 수
#define SIZE 0.3

glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 10.0f);

glm::mat4 lt = glm::mat4(1.0f);
glm::vec3 ltPos = glm::vec3(0.0f, 1000.0f, 0.0f);
glm::vec4 ltPos4 = glm::vec4(ltPos, 1.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::vec3 LC = glm::vec3(1.0f, 1.0f, 1.0f);


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

unsigned int modelLocation, projectionLocation, viewLocation, lightPosLocation, lightColorLocation, viewposLocation, vColorLocation, flagLocation;

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

struct Rec {
	GLuint vao[6], vbo[6][3], ebo;

	float* vPosData;
	float* vNormalData;
	float* vTextureCoordinateData;
	int* indexData;
	int vertexCount;
	int indexCount;

	int width, height, nrChannels;
	unsigned int texture[6];
	glm::mat4 S1 = glm::mat4(1.0f);

	void LoadObj()
	{
		ReadObj("rec.obj", vPosData, vNormalData, vTextureCoordinateData, indexData, vertexCount, indexCount);
	}

	void InitBuffer()
	{
		for (int i = 0; i < 6; i++)
		{
			glGenVertexArrays(1, &vao[i]);
			glBindVertexArray(vao[i]);

			glGenBuffers(3, vbo[i]);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);
      		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16 , &vPosData[i*12], GL_STATIC_DRAW);
			int posLocation = glGetAttribLocation(s_program, "in_Position");
			glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 0, NULL);
			glEnableVertexAttribArray(posLocation);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);
			glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, &vNormalData[i * 3 ], GL_STATIC_DRAW);
			int normalLocation = glGetAttribLocation(s_program, "in_Normal");
			glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
			glEnableVertexAttribArray(normalLocation);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][2]);
			glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 2, &vTextureCoordinateData[i * 2 ], GL_STATIC_DRAW);
			int uvLocation = glGetAttribLocation(s_program, "in_uv");
			glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
			glEnableVertexAttribArray(uvLocation);

		}

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indexData, GL_STATIC_DRAW);

		S1 = glm::scale(S1, glm::vec3(0.03, 0.03, 0.03));
	}	void InitTexture()
	{
		for (int i = 0; i < 6; i++)
		{
			glGenTextures(1, &texture[i]); //--- 텍스처 생성
			glBindTexture(GL_TEXTURE_2D, texture[i]); //--- 텍스처 바인딩
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			unsigned char *data = stbi_load("box0.png", &width, &height, &nrChannels, 0); //--- 텍스처로 사용할 비트맵 이미지 로드하기
			glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의
			glUseProgram(s_program);
			int tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
			glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
		}
		
	}
	void Draw()
	{
		for (int i = 0; i < 6; i++)
		{
			glBindVertexArray(vao[0]);
			glUniform3f(vColorLocation, 1.0f, 1.0f, 1.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glUniform1i(flagLocation, 0);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R*S1));
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		}
		
	}
};

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


void InitRotate()
{
	R = glm::mat4(1.0f);

	Rx = glm::mat4(1.0f);
	Ry = glm::mat4(1.0f);


	Rx = glm::rotate(Rx, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	Ry = glm::rotate(Ry, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));

	R = Rx * Ry;

}

Rec r;

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
	r.LoadObj();
	r.InitTexture();
	r.InitBuffer();




	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard);
	glutSpecialFunc(ArrowKey);
	glutMainLoop(); // 이벤트 처리 시작 
}

void DrawCL()
{
	glm::mat4 cl{ 1.0f };
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cl));

	glBegin(GL_LINES);
	glVertex2f(-1.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.0f, 1.0f);
	glVertex2f(0.0f, -1.0f);
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

GLvoid TimerFunction(int value)
{
	if (stop) return;

	Rotate();
	glutPostRedisplay();
	glutTimerFunc(t, TimerFunction, 1);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	glUseProgram(s_program);
	vColorLocation = glGetUniformLocation(s_program, "Colors");
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	viewLocation = glGetUniformLocation(s_program, "viewTransform");
	viewposLocation = glGetUniformLocation(s_program, "viewPos");
	lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	flagLocation = glGetUniformLocation(s_program, "flag");
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 50.0f);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	glUniform3f(viewposLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	glUniform3f(lightPosLocation, ltPos4.x, ltPos4.y, ltPos4.z);
	glUniform3f(lightColorLocation, LC.x, LC.y, LC.z);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	if (!w)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	DrawCL();

	if (one++ == 0)InitRotate();


	if (select == 0) r.Draw();
	if (select == 1) glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glutSwapBuffers(); // 화면에 출력하기

}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}