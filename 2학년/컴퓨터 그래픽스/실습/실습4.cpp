#include <iostream>
#include <random>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define COUNT 4

using namespace std;



GLint index = 0;
GLboolean click = 0;
GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
struct Triangle {
	GLfloat triShape[3][3];
	GLfloat colors[3][3] = { {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0} };

	Triangle(float x, float y)
	{
		float size = 0.09;

		triShape[0][0] = x;
		triShape[0][1] = y;
		triShape[0][2] = 0.0;

		triShape[1][0] = x - size;
		triShape[1][1] = y - sqrt(3) * size;
		triShape[1][2] = 0.0;

		triShape[2][0] = x + size;
		triShape[2][1] = y - sqrt(3) * size;
		triShape[2][2] = 0.0;

	}

	void setPos(float x, float y)
	{
		float size = 0.09;

		triShape[0][0] = x;
		triShape[0][1] = y;
		triShape[0][2] = 0.0;

		triShape[1][0] = x - size;
		triShape[1][1] = y - sqrt(3) * size;
		triShape[1][2] = 0.0;

		triShape[2][0] = x + size;
		triShape[2][1] = y - sqrt(3) * size;
		triShape[2][2] = 0.0;
	}

};

Triangle tri[COUNT] = { Triangle(-0.5,0.8),Triangle(0.5,0.8),Triangle(-0.5,-0.4),Triangle(0.5,-0.4) };

int winWidth = 800, winHeight = 600;

char* filetobuf(const char *file)
{
	FILE *fptr;
	long length;
	char *buf;

	fopen_s(&fptr,file, "rb");
	if(!fptr) return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}

void converDeviceXYOpenGLXY(int x, int y, float* ox, float* oy)
{
	int w = winWidth;
	int h = winHeight;
	*ox = (float)(x - (float)w / 2.0)*(float)(1.0 / (float)(w / 2.0));
	*oy = -(float)(y - (float)h / 2.0)*(float)(1.0 / (float)(h / 2.0));
}



GLchar *vertexsource, *fragmentsource;
GLuint vertexshader, fragmentshader;

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

GLuint vao, vbo[2];

GLuint s_program;

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

void InitBuffer(int index)
{
	glGenVertexArrays(1, &vao);

	glGenBuffers(2, vbo);

	glBindVertexArray(vao);

	//위치
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(tri[index].triShape), tri[index].triShape, GL_STATIC_DRAW); // 여러개 입력?

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // 문제?

	glEnableVertexAttribArray(0);

	//색상
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(tri[index].colors), tri[index].colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
}

 



void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기

	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard);
	glutMouseFunc(Mouse);

	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float ox, oy;

	converDeviceXYOpenGLXY(x, y, &ox, &oy);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		click = 1;
		
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		tri[index++].setPos(ox, oy);
		if (index % 4 == 0) index = 0;
		click = 0;
	}
}

GLvoid KeyBoard(unsigned char key, int x, int y)
{

	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{

}


GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0,1.0,1.0,1.0f); // 설정된 색으로 전체를 칠하기
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	for (int i = 0; i < COUNT; i++)
	{
		InitBuffer(i);
		glUseProgram(s_program);
		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	

	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}