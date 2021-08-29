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
#define COUNT 7
#define RC 6
#define SLICE 10
//#define SIZE 0.3


GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid ArrowKey(int, int, int);
GLint t = 10, dt = 0;
GLboolean  m = true ,stop = true;

glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);glm::mat4 Space = glm::mat4(1.0f);glm::mat4 SpaceTx = glm::mat4(1.0f);
glm::mat4 SpaceTy = glm::mat4(1.0f);
glm::mat4 SpaceTz = glm::mat4(1.0f);
glm::mat4 SpaceRy = glm::mat4(1.0f);


glm::mat4 Rs1 = glm::mat4(1.0f);
glm::mat4 Rs2 = glm::mat4(1.0f);
glm::mat4 Rs3 = glm::mat4(1.0f);

glm::mat4 T = glm::mat4(1.0f);
glm::mat4 Rx = glm::mat4(1.0f);
glm::mat4 Ry = glm::mat4(1.0f);
glm::mat4 Rz = glm::mat4(1.0f);

glm::mat4 R2 = glm::mat4(1.0f);
glm::mat4 R2x = glm::mat4(1.0f);
glm::mat4 Rm = glm::mat4(1.0f);

glm::mat4 R3x = glm::mat4(1.0f);

glm::mat4 Tx = glm::mat4(1.0f);
glm::mat4 Ty = glm::mat4(1.0f);
unsigned int modelLocation,projectionLocation,viewLocation;

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




/*void UpdateBuffer()
{
	for (int i = 0; i < COUNT; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);


		if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(), , GL_STATIC_DRAW);
		else if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(), , GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);
	}
}*/

struct Sphere {
	float radius;
	float speed;
	GLUquadricObj *qobj;
	glm::mat4 Rms = glm::mat4(1.0f);

	Sphere()
	{
		qobj = gluNewQuadric();

	}
	~Sphere()
	{
		gluDeleteQuadric(qobj);
	}
	void setRS(float r,float s)
	{
		radius = r;
		speed = s;
	}

	void Rotate(int type)
	{
		if(type >= 1)Rms = glm::rotate(Rms, glm::radians(speed * 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//else if(type >= 2)Rms = glm::rotate(Rms, glm::radians(speed * 1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		
	}

	void draw(int type)
	{
		glUseProgram(s_program);
		
		if (type >= 1)
		{
			
			Tx = glm::mat4(1.0f);
			Rz = glm::mat4(1.0f);
			if (type == 1)
			{
				Rx = glm::rotate(Rx, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				Ry = glm::mat4(1.0f);
				Tx = glm::translate(Tx, glm::vec3(-0.8f, 0.0f, 0.0f));
				Rs1 =  Rx * Rms * Tx;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space * Rs1));
			}
			if (type == 2)
			{
				
				Tx = glm::translate(Tx, glm::vec3(-0.8f, 0.0f, 0.0f));
				Rz = glm::rotate(Rz, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				Rs2 = Rx * Rms * Tx;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space *Rz * Rs2 ));
			}
			if (type == 3)
			{
				Tx = glm::translate(Tx, glm::vec3(-0.8f, 0.0f, 0.0f));
				Rz = glm::rotate(Rz, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				Rs3 = Rx * Rms * Tx;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space *Rz * Rs3));
			}
			if (type == 4)
			{
				
				Tx = glm::translate(Tx, glm::vec3(-0.25f, 0.0f, 0.0f));
				Ry = glm::mat4(1.0f);
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space *Rs1 * Rms *Tx));
			}
			if (type == 5)
			{
				Rz = glm::rotate(Rz, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				Tx = glm::translate(Tx, glm::vec3(-0.15f, 0.15f, 0.0f));
				Ry = glm::mat4(1.0f);
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space *Rz * Rs2 * Rz * Rms *Tx));
			}
			if (type == 6)
			{
				Rz = glm::rotate(Rz, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				Tx = glm::translate(Tx, glm::vec3(-0.15f, -0.15f, 0.0f));
				Ry = glm::mat4(1.0f);
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space *Rz * Rs3 * Rz * Rms *Tx));
			}
		}
		
		if (m)gluQuadricDrawStyle(qobj, GLU_LINE);
		else gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		

		int vColorLocation = glGetUniformLocation(s_program, "outColor");
		
		if(type == 0)glUniform4f(vColorLocation, 0.0f, 0.0f, 0.8f, 1.0);
		else if(type >= 1 && type <= 3)glUniform4f(vColorLocation, 0.8f, 0.5f, 0.0f, 1.0);
		else if(type > 3) glUniform4f(vColorLocation, 0.0f, 0.0f, 0.5f, 1.0);
		gluSphere(qobj, radius, 50, SLICE);
	}
};

Sphere s[COUNT];

struct Route {
	float radius;
	float pos[360][3];
	float color[360][3] = { 0.0f, };
	int type = 0;
	void init()
	{
		for (int i = 0; i < 360; i++)
		{
			pos[i][0] = radius * cos(glm::radians((float)i));
			pos[i][1] = radius * sin(glm::radians((float)i));
			pos[i][2] = 0.0f;
		}
		
	}

	void setR(float r)
	{
		radius = r;
	}
	void setType(int t)
	{
		type = t;
	}
	
	void draw(int n)
	{
		int vColorLocation = glGetUniformLocation(s_program, "outColor");
		glUseProgram(s_program);
		glUniform4f(vColorLocation, 0.0f, 0.0f, 0.0f, 1.0);

		Rx = glm::mat4(1.0f);
		Ry = glm::mat4(1.0f);
		Rz = glm::mat4(1.0f);
		Tx = glm::mat4(1.0f);
		Rx = glm::rotate(Rx, glm::radians(80.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		if (type == 1)Ry = glm::rotate(Ry, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		if (type == 2)Ry = glm::rotate(Ry, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		if( type <= 2)glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space *Rx*Ry));
		if (type == 3)
		{
			R2x = glm::rotate(Rx, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 tz = glm::mat4(1.0f);
			tz = glm::rotate(tz, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			
			R2 = Rs1 * R2x;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space *R2));
		}
		if (type == 4)
		{
			
			Rz = glm::rotate(Rz, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			
			R2 = Rs3 * R2x;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space *Rz * R2));
		}
		if (type == 5)
		{

			Rz = glm::rotate(Rz, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			R2 = Rs2 * R2x;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space *Rz * R2));
		}
		glBindVertexArray(vao[n]);
		glDrawArrays(GL_LINE_LOOP, 0, 360);
	}

};

Route r[RC];

void InitBuffer()
{

	for (int i = 0; i < RC; i++)
	{
		glGenVertexArrays(1, &vao[i]);

		glBindVertexArray(vao[i]);
		glGenBuffers(2, vbo[i]);

		//위치
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(r[i].pos), r[i].pos, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		//색상
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(r[i].color), r[i].color, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);

		//EBO 설정
		/*
		glGenBuffers(1, &ebo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		if (i == 0) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(), , GL_STATIC_DRAW);
		if (i == 1) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(), , GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		*/

	}

}
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA ); // 디스플레이 모드 설정
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

	s[0].setRS(0.25,0);

	s[1].setRS(0.1,2);
	s[2].setRS(0.1,3);
	s[3].setRS(0.1,5);
	s[4].setRS(0.05,2);
	s[5].setRS(0.05,3);
	s[6].setRS(0.05,5);
	for (int i = 0; i < 3; i++)
	{
		r[i].setR(0.8);
		r[i].setType(i);
		r[i].init();
	}
	for (int i = 3; i < RC; i++)
	{
		r[i].setR(0.25);
		r[i].setType(i);
		r[i].init();
	}

	InitShader();
	InitBuffer();
	
	
	


	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard);
	glutSpecialFunc(ArrowKey);
	glutTimerFunc(t, TimerFunction, 1);
	glutMainLoop(); // 이벤트 처리 시작 
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
	if (key == 'm' || key == 'M')
	{
		if (m == false) m = true;
		else if (m == true) m = false;
		
	}
	if (key == 'w')
	{
		SpaceTy = glm::translate(SpaceTy, glm::vec3(0.0f, 0.1f, 0.0f));
	}
	if (key == 'a')
	{
		SpaceTx = glm::translate(SpaceTx, glm::vec3(-0.1f, 0.0f, 0.0f));
	}
	if (key == 's')
	{
		SpaceTy = glm::translate(SpaceTy, glm::vec3(0.0f, -0.1f, 0.0f));
	}
	if (key == 'd')
	{
		SpaceTx = glm::translate(SpaceTx, glm::vec3(0.1f, 0.0f, 0.0f));
	}

	if (key == 'z')
	{
		SpaceTz = glm::translate(SpaceTz, glm::vec3(0.0f, 0.0f, 0.1f));
	}
	if (key == 'x')
	{
		SpaceTz = glm::translate(SpaceTz, glm::vec3(0.0f, 0.0f, -0.1f));
	}
	if (key == 'y')
	{
		SpaceRy = glm::rotate(SpaceRy,glm::radians(30.0f) ,glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (key == 'Y')
	{
		SpaceRy = glm::rotate(SpaceRy,  glm::radians(-30.0f) , glm::vec3(0.0f, 1.0f, 0.00f));
	}

	Space = SpaceRy * SpaceTx * SpaceTy * SpaceTz;

	glutPostRedisplay();
}



GLvoid TimerFunction(int value)
{
	

	for (int i = 1; i <= 3; i++)
	{
		s[i].Rotate(i);
	}
	
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
	
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	viewLocation = glGetUniformLocation(s_program, "viewTransform");

	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	if (m == true)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if(m == false) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	
	

	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Space));
	s[0].draw(0);
	for (int i = 0; i < RC; i++)
	{
		
		r[i].draw(i);
	}

	for (int i = 1; i < COUNT; i++)
	{
		
		s[i].draw(i);
	}
	
	
	
	

	glutSwapBuffers(); // 화면에 출력하기

}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}