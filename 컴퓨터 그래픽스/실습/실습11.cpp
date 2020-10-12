#include <iostream>
#include <random>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define COUNT 1
#define WIDTH 500
#define HEIGHT 500
#define SIZE 0.05
#define speed 0.01
using namespace std;


void converDeviceXYOpenGLXY(float& x, float& y)
{
	int w = WIDTH;
	int h = HEIGHT;
	x = (float)(x - (float)w / 2.0)*(float)(1.0 / (float)(w / 2.0));
	y = -(float)(y - (float)h / 2.0)*(float)(1.0 / (float)(h / 2.0));
}

GLboolean stop = true, lbutton = false;

struct Tri {
	float pos[4][3];
	float color[4][3];
	float cp[4][2][3];
	

	Tri()
	{
		pos[0][0] = -0.5;
		pos[0][1] = 0.5;

		pos[1][0] = 0.5;
		pos[1][1] = 0.5;
		
		pos[2][0] = 0.5;
		pos[2][1] = -0.5;
		
		pos[3][0] = -0.5;
		pos[3][1] = -0.5;
		for (int i = 0; i < 4; i++)pos[i][2] = 0.0f;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 3; j++)color[i][j] = 0.5f;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 2; j++) cp[i][j][2] = 0.0f;
	}

	void setcp()
	{
		for (int i = 0; i < 4; i++)
		{
			
				cp[i][0][0] = pos[i][0] - SIZE;
				cp[i][0][1] = pos[i][1] + SIZE;
				
				cp[i][1][0] = pos[i][0] + SIZE;
				cp[i][1][1] = pos[i][1] - SIZE;
				
		}
		
	}

	void draw()
	{
		
		
	}

	void move()
	{
		                                              
	}
};

GLint time = 10, point;

GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid Reshape(int w, int h);

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
GLuint vao[COUNT], vbo[COUNT][2];
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

Tri s ;

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

		glBufferData(GL_ARRAY_BUFFER, sizeof(s.pos), s.pos, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		//색상
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(s.color), s.color, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
	}

}


void UpdateBuffer()
{
	for (int i = 0; i < COUNT; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(s.pos), s.pos, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);
	}
}
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(WIDTH, HEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)
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
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);


	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float cx = x, cy = y;
	converDeviceXYOpenGLXY(cx, cy);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i < 4; i++)
		{
			if (s.pos[i][0] - SIZE < cx &&s.pos[i][0] + SIZE > cx)
			{
				if (s.pos[i][1] - SIZE < cy && s.pos[i][1] + SIZE > cy)
				{
					lbutton = true;
					point = i;
				}
			}
		}
		
		
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		lbutton = false;
		point = -1;
	}
	glutPostRedisplay();
}
GLvoid Motion(int x, int y)
{
	float cx = x, cy = y;
	converDeviceXYOpenGLXY(cx, cy);

	if (lbutton == true && point>-1)
	{
		s.pos[point][0] = cx;
		s.pos[point][1] = cy;
		UpdateBuffer();
	}
	glutPostRedisplay();
}

GLvoid KeyBoard(unsigned char key, int x, int y)
{

	if (key == 'a' || key == 'A')
	{
		stop = false;
		glutTimerFunc(time, TimerFunction, 1);
	}
	if (key == 's' || key == 'S')
	{
		stop = true;
	}

	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (stop == true) return;

	for (int i = 1; i < COUNT; i++)
	{
		
	}
	UpdateBuffer();



	glutPostRedisplay();
	glutTimerFunc(time, TimerFunction, 1);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현

	glUseProgram(s_program);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	//--- 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}