#include <iostream>
#include <random>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define PI 3.14159265359
#define COUNT 10
using namespace std;

int winWidth = 600, winHeight = 600;

GLint index = 0, time = 10;
GLboolean click = 0, f = 0;
GLfloat speed = 0.001f, black[3] = { 0.0,0.0,0.0 };
GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);

GLchar *vertexsource, *fragmentsource;
GLuint vertexshader, fragmentshader;
GLuint vao, vbo[2];
GLuint s_program;

float getradian(float a)
{
	return (a * PI / 180.f);
}

void converDeviceXYOpenGLXY(int x, int y, float* ox, float* oy)
{
	int w = winWidth;
	int h = winHeight;
	*ox = (float)(x - (float)w / 2.0)*(float)(1.0 / (float)(w / 2.0));
	*oy = -(float)(y - (float)h / 2.0)*(float)(1.0 / (float)(h / 2.0));
}

struct CIR {
	int type,blank = 0;
	float r = 0;
	float center[2] = { 0, };
	float angle[360][3];

	CIR()
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> uid(0, 1);

		type = uid(mt);
		

	}

	void setCen(float x, float y)
	{
		center[0] = x;
		center[1] = y;
	}
	
	void move()
	{
		r += speed;

		for (float i = 0.f; i < 360.f; i += 1.0f)
		{
			angle[(int)i][0] = cos(getradian(i)) * r + center[0];
			angle[(int)i][1] = sin(getradian(i)) * r + center[1];
			angle[(int)i][2] = 0.0;
		}
	}

	void reset()
	{
		memset(angle, 0, sizeof(angle));
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> uid(0, 1);

		type = uid(mt);
		
	}

	void update() 
	{
		if (blank == 0);
		move();
		if (type == 0) // ���������
		{
			if (r > 1.1)
			{
				blank = 0;
				reset();
			}
		}


		if (type == 1)
		{
			if (r > 0.1) r = 0.0f;
		}

	}
};

CIR c[10];

char* filetobuf(const char *file)
{
	FILE *fptr;
	long length;
	char *buf;

	fopen_s(&fptr,file, "rb");
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
		cerr << "ERROR : vertex shader �����Ͻ���\n" << errorLog << endl;

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
		cerr << "ERROR : vertex shader �����Ͻ���\n" << errorLog << endl;

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

void InitBuffer(CIR c)
{
	glGenVertexArrays(1, &vao);

	glGenBuffers(2, vbo); // ������Ʈ���� �߰�

	glBindVertexArray(vao);

	//��ġ
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(c.angle),c.angle , GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(black),black , GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	
}

void UpdateBuffer()
{
	glDeleteBuffers(1, &vao);
}



void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(winWidth, winWidth); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����(������ �̸�)
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	InitShader();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(KeyBoard);
	glutMouseFunc(Mouse);
	glutTimerFunc(time, TimerFunction, 1);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
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
		if (c[index].blank == 1) c[index].reset();
		c[index].setCen(ox, oy);
		c[index].blank = 1;
		if(index % 10 == 0) index = 0;
		index++;
		click = 0;
	}
}

GLvoid KeyBoard(unsigned char key, int x, int y)
{
	if (key == 'f' || key == 'F')
	{
		if (!f) f = true;
		else f = false;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	for (int i = 0; i < COUNT; i++)
	{
		if (c[i].blank == 0) continue;
		c[i].update();
	}
	glutPostRedisplay();
	glutTimerFunc(time, TimerFunction, 1);
	cout << time << endl;
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����

	for (int i = 0; i < COUNT; i++)
	{
		cout << i << " " << c[i].blank << endl;
		if (c[i].blank == 0) continue;
		if(f) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		InitBuffer(c[i]);
		glUseProgram(s_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_POLYGON, 0, 360);
	
	}
	
	
	UpdateBuffer();
	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}