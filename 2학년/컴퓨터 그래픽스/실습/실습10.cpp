#include <iostream>
#include <random>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define COUNT 5
#define WIDTH 500
#define HEIGHT 500
#define BASE 0.1
#define SIZE 0.05
#define speed 0.01
using namespace std;

enum vtx { // NSWE NW NE SW SE
	GRID,
	LINE,
	TRI,
	REC,
	PEN,
	CIR
};

GLboolean stop = true;

struct Shape {
	float pos[6][3];
	float color[6][3];
	vtx type;
	int point;

	Shape(vtx t)
	{
		type = t;

		if (type == GRID) setGrid();
		if (type == LINE) setLine();
		if (type == TRI) setTri();
		if (type == REC) setRec();
		if (type == PEN) setPen();
	}

	void setGrid()
	{
		
		point = 4;
		pos[0][0] = -1.0f;
		pos[0][1] = 0.0f;

		pos[1][0] = 1.0f;
		pos[1][1] = 0.0f;

		pos[2][0] = 0.0f;
		pos[2][1] = 1.0f;

		pos[3][0] = 0.0f;
		pos[3][1] = -1.0f;
		
		for (int i = 0; i < point; i++) pos[i][2] = 0.0f;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				color[i][j] = 0.0f;
			}
		}
	}
	void setLine()
	{
		point = 3;
		
		pos[0][0] = -0.9f;
		pos[0][1] = 0.2f;
		
		pos[1][0] = -0.5f;
		pos[1][1] = 0.55f;

		pos[2][0] = -0.1f;
		pos[2][1] = 0.9f;

		for (int i = 0; i < point; i++) pos[i][2] = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				color[i][j] = 0.3f;
			}
		}
	}
	void setTri()
	{
		point = 4;

		pos[0][0] = 0.1f;
		pos[0][1] = 0.1f;

		pos[1][0] = 0.9f;
		pos[1][1] = 0.1f;

		pos[2][0] = 0.5f;
		pos[2][1] = 0.8f;

		pos[3][0] = 0.5f;
		pos[3][1] = 0.8f;

		for (int i = 0; i < point; i++) pos[i][2] = 0.0f;
		for (int i = 0; i < point; i++)
		{
			
				color[i][0] = 0.7f;
			
		}
	}
	void setRec()
	{
		point = 5;

		pos[0][0] = -0.1f;
		pos[0][1] = -0.1f;

		pos[1][0] = -0.1f;
		pos[1][1] = -0.8f;

		pos[2][0] = -0.9f;
		pos[2][1] = -0.8f;

		pos[3][0] = -0.9f;
		pos[3][1] = -0.1f;

		pos[4][0] = -0.5f;
		pos[4][1] = -0.1f;

		for (int i = 0; i < point; i++) pos[i][2] = 0.0f;
		for (int i = 0; i < point; i++)
		{

			color[i][1] = 0.7f;

		}
	}
	void setPen()
	{
		point = 5;

		pos[0][0] = 0.5f;
		pos[0][1] = -0.1f;

		pos[1][0] = 0.8f;
		pos[1][1] = -0.4f;

		pos[2][0] = 0.65f;
		pos[2][1] = -0.7f;

		pos[3][0] = 0.35f;
		pos[3][1] = -0.7f;

		pos[4][0] = 0.2f;
		pos[4][1] = -0.4f;

		for (int i = 0; i < point; i++) pos[i][2] = 0.0f;
		for (int i = 0; i < point; i++)
		{

			color[i][2] = 0.7f;

		}
	}

	void draw()
	{
		switch (type)
		{
		case GRID:
			glDrawArrays(GL_LINES, 0, point);
			break;
		case LINE:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if(pos[1][0] == -0.5f) glDrawArrays(GL_LINE_STRIP, 0, point);
			else glDrawArrays(GL_TRIANGLES, 0, point);
			break;
		case TRI:
		case REC:
		case PEN:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_POLYGON, 0, point);
			break;
		}
	}

	void move()
	{
		switch (type)
		{
		case LINE:
			pos[1][0] += -1 * speed;
			pos[1][1] += 1 * speed;
			if (pos[1][0]<-1.0 || pos[1][1] > 1.0) setLine();
			break;
		case TRI:
			pos[3][0] += -1 * speed;
			pos[2][0] += 1 * speed;
			if (pos[2][0]> 1.0 || pos[3][0] < 0.0) setTri();
			break;
		case REC:
			pos[1][0] += -1 * speed;
			pos[2][0] += 1 * speed;
			pos[0][1] += -1 * speed;
			pos[3][1] += -1 * speed;
			if (pos[1][0] < -0.4 || pos[2][0] > -0.6) setRec();
			break;
		case PEN:
			
			pos[0][1] += -1 * speed * BASE;

			pos[1][0] += -1 * speed* BASE;


			pos[2][0] += -1 * speed* 0.05;
			pos[2][1] += 1 * speed* 0.1;

			pos[3][0] += 1 * speed* 0.05;
			pos[3][1] += 1 * speed* 0.1;

			pos[4][0] += 1 * speed* BASE;
			if (pos[0][1] < -0.4 || pos[2][0] < 0.5)setPen();
			break;
		}
	}
};

GLint time = 10;

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

Shape s[COUNT] = { Shape(GRID), Shape(LINE),Shape(TRI),Shape(REC),Shape(PEN) };

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

		glBufferData(GL_ARRAY_BUFFER, sizeof(s[i].pos), s[i].pos, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		//����
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(s[i].color), s[i].color, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
	}

}


void UpdateBuffer()
{
	for (int i = 0; i < COUNT; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(s[i].pos), s[i].pos, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);
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
		s[i].move();
	}
	UpdateBuffer();

	

	glutPostRedisplay();
	glutTimerFunc(time, TimerFunction, 1);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����

	glUseProgram(s_program);
	for (int i = 0; i <COUNT; i++)
	{
		glBindVertexArray(vao[i]);
		s[i].draw();
	}

	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}