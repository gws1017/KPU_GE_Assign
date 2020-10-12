#include <iostream>
#include <random>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define COUNT 4
#define WIDTH 800
#define HEIGHT 600
#define BASE 0.1
#define SIZE 0.05
#define SPEED 0.01
using namespace std;

enum dir { // NSWE NW NE SW SE
	N,
	S,
	W,
	E,
	NW,
	NE,
	SW,
	SE,
	NONE
};

struct triangle {
	GLfloat trishape[3][3];
	float temp[3][3];
	float color[3][3];
	dir d, state = NONE; // �������� ����
	int dx, dy,vdl =1, vdr = -1;
	float size=0; 
	triangle(dir _d)
	{
		d = _d;
		setColor();
		setPos();
		
		setdir();

	}

	void setColor()
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> uidC(0, 255);

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				color[i][j] = uidC(mt) * 1.0f / 255.0f;
			}
		}
			
	}



	void setPos()
	{

		trishape[0][0] = 0.0f;
		trishape[0][1] = 0.0f;
		trishape[0][2] = 0.0f;
		switch (d)
		{
		case N:
			trishape[1][0] = 0.5f;
			trishape[1][1] = 0.5f;
			trishape[1][2] = 0.0f;

			trishape[2][0] = -0.5f;
			trishape[2][1] = 0.5f;
			trishape[2][2] = 0.0f;
			break;
		case W:
			trishape[1][0] = -0.5f;
			trishape[1][1] = 0.5f;
			trishape[1][2] = 0.0f;

			trishape[2][0] = -0.5f;
			trishape[2][1] = -0.5f;
			trishape[2][2] = 0.0f;
			break;
		case S:
			trishape[1][0] = -0.5f;
			trishape[1][1] = -0.5f;
			trishape[1][2] = 0.0f;

			trishape[2][0] = 0.5f;
			trishape[2][1] = -0.5f;
			trishape[2][2] = 0.0f;
			break;
		case E:
			trishape[1][0] = 0.5f;
			trishape[1][1] = -0.5f;
			trishape[1][2] = 0.0f;

			trishape[2][0] = 0.5f;
			trishape[2][1] = 0.5f;
			trishape[2][2] = 0.0f;
			break;
		}
		



	}
	void setdir()
	{

		switch (d)
		{
		case N:
			dx = 0;
			dy = 1;
			break;
		case S:
			dx = 0;
			dy = -1;
			break;
		case E:
			dx = 1;
			dy = 0;
			break;
		case W:
			dx = -1;
			dy = 0;
			break;
		case NW:
			dx = -1;
			dy = 1;
			break;
		case NE:
			dx = 1;
			dy = 1;
			break;
		case SW:
			dx = -1;
			dy = -1;
			break;
		case SE:
			dx = 1;
			dy = -1;
			break;
		}


	}
	void move()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				temp[i][j] = trishape[i][j];
			}
		}
		for (int i = 0; i < 3; i++)
		{
			
				trishape[i][0] += dx * SPEED;
				trishape[i][1] += dy * SPEED;
		}
		
	}
	void sizefunc()
	{
			
		switch (d)
		{
		case N:
		case S:
			for (int i = 1; i <= 2; i++)
			{
				if (trishape[i][0] > 0.6)
				{
					vdl *= -1;
					vdr *= -1;
				}
			}
			trishape[1][0] += vdl * SPEED;
			trishape[2][0] += vdr * SPEED;
			break;
		case E:
		case W:
			for (int i = 1; i <= 2; i++)
			{
				if (trishape[i][1] > 0.6)
				{
					vdl *= -1;
					vdr *= -1;
				}
			}
			trishape[1][1] += vdl * SPEED;
			trishape[2][1] += vdr * SPEED;
			break;
		}
	}
	void collpse()
	{
		
		switch (d)
		{
		case N:
			if (trishape[1][1] > 1.0 || trishape[2][1] > 1.0)dy *= -1;
			if (trishape[0][1] <= 0.0f)dy *= -1;
			break;
		case S:
			if (trishape[1][1] < -1.0 || trishape[2][1] < -1.0)dy *= -1;
			if (trishape[0][1] >= 0.0f)dy *= -1;
			break;
		case E:
			if (trishape[1][0] > 1.0 || trishape[2][0] > 1.0)dx *= -1;
			if (trishape[0][0] <= 0.0f)dx *= -1;
			break;
		case W:
			if (trishape[1][0] < -1.0 || trishape[2][0] < -1.0)dx*= -1;
			if (trishape[0][0] >= 0.0f)dx *= -1;
			break;
		}	

	}
	void update()
	{
		
		move();
		collpse();
	}
	

};

triangle tri[COUNT] = { triangle(N),triangle(W),triangle(S),triangle(E) };

GLint index = 0, time = 10, frame = 0;
GLboolean stop = 1, sz = 0;
GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
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

void InitBuffer()
{
	for (int i = 0; i < COUNT; i++)
	{
		glGenVertexArrays(1, &vao[i]);

		glBindVertexArray(vao[i]);
		glGenBuffers(2, vbo[i]);


		//��ġ
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, tri[i].trishape, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		//����
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, tri[i].color, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
	}

}


void UpdateBuffer()
{
	for (int i = 0; i < COUNT; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, tri[i].trishape, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, tri[i].color, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
	}
}
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(WIDTH, HEIGHT); // �������� ũ�� ����
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
	InitBuffer();
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(KeyBoard);


	glutMainLoop(); // �̺�Ʈ ó�� ���� }
}


GLvoid KeyBoard(unsigned char key, int x, int y)
{

	if (key == 's' || key == 'S')
	{
		sz = true;
	}
	if (key == 't' || key == 'T')
	{
		if (!stop) stop = true;
		else
		{
			stop = false;
			sz = false;
		}
		glutTimerFunc(time, TimerFunction, 1);
	}


	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (stop == 1) return;

	for (int i = 0; i < COUNT; i++)
	{
		tri[i].update();
		if (sz == true)tri[i].sizefunc();
	}
	UpdateBuffer();

	glutPostRedisplay();
	glutTimerFunc(time, TimerFunction, 1);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����

	glUseProgram(s_program);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int i = 0; i < COUNT; i++)
	{
		glBindVertexArray(vao[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}