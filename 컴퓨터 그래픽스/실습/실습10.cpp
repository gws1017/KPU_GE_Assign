#include <iostream>
#include <random>
#include <gl/glew.h> // 필요한 헤더파일 include
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


		//위치
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(s[i].pos), s[i].pos, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		//색상
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


	glutMainLoop(); // 이벤트 처리 시작 }
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

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현

	glUseProgram(s_program);
	for (int i = 0; i <COUNT; i++)
	{
		glBindVertexArray(vao[i]);
		s[i].draw();
	}

	//--- 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}