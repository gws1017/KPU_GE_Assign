#include <iostream>
#include <random>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define COUNT 10
#define WIDTH 500
#define HEIGHT 500
#define BASE 0.1
#define SIZE 0.05

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
	float color[3];
	float g[2];
	float speed;
	dir d, state = NONE;
	int dx, dy;
	int type; // 1이면 내부 0이면 외부
	triangle(int a)
	{
		type = a;
		setColor();
		setG();
		setS();
		setPos();
		random_device rd;
		mt19937 mt(rd());
		if (type == 1)
		{
			uniform_int_distribution<int> uidD2(4, 7);
			d = (dir)uidD2(mt);
		}
		else d = NE;
		setdir();

	}

	void setColor()
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> uidC(0, 255);

		for (int i = 0; i < 3; i++)
			color[i] = uidC(mt) * 1.0f / 255.0f;
	}

	void setS()
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> uidS(1, 1);

		speed = uidS(mt) / 100.0f;
	}

	void setG()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> uidG(-100,-50), uidG2(-10,10);
		if (type == 1)
		{
			g[0] = uidG2(mt) / 22.2222f;
			g[1] = uidG2(mt) / 22.2222f; 
		}

		if (type == 0)
		{
		g[0] = 0.6;
		g[1] = 0.0f;
		}
	}

	void setPos()
	{
		
		
			trishape[0][0] = g[0];
			trishape[0][1] = g[1] + SIZE * 2;
			trishape[0][2] = 0.0f;

			trishape[1][0] = (g[0] - BASE / 2);
			trishape[1][1] = (g[1] - SIZE);
			trishape[1][2] = 0.0f;

			trishape[2][0] = (g[0] + BASE / 2);
			trishape[2][1] = (g[1] - SIZE);
			trishape[2][2] = 0.0f;
		
		

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
		g[0] += dx * speed;
		g[1] += dy * speed;

	}
	void collpse()
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> uidD(4, 7);

		int t;

		for (int i = 0; i < 3; i++)
		{

			if (type == 1)
			{
				if (trishape[i][0] < -0.5) //WEST
				{
					t = uidD(mt);
					while (t == W && t == NW && t == SW) t = uidD(mt);
					d = (dir)t;
					setdir();
					state = W;

					g[0] = -0.5 + SIZE;

					trishape[0][0] = g[0] + SIZE * 2;
					trishape[0][1] = g[1];

					trishape[1][0] = (g[0] - SIZE);
					trishape[1][1] = (g[1] - BASE / 2);


					trishape[2][0] = (g[0] - SIZE);
					trishape[2][1] = (g[1] + BASE / 2);
					setColor();
				}
				if (trishape[i][0] > 0.5) //EAST
				{
					t = uidD(mt);
					while (t == E && t == NE && t == SE) t = uidD(mt);
					d = (dir)t;
					setdir();
					state = E;

					g[0] = 0.5 - SIZE;

					trishape[0][0] = g[0] - SIZE * 2;
					trishape[0][1] = g[1];


					trishape[1][0] = (g[0] + SIZE);
					trishape[1][1] = (g[1] - BASE / 2);


					trishape[2][0] = (g[0] + SIZE);
					trishape[2][1] = (g[1] + BASE / 2);
					setColor();
				}
				if (trishape[i][1] > 0.5) //NORTH
				{
					t = uidD(mt);
					while (t == N && t == NW && t == NE) t = uidD(mt);
					d = (dir)t;
					setdir();
					state = N;

					g[1] = 0.5 - SIZE;

					trishape[0][0] = g[0];
					trishape[0][1] = g[1] - SIZE * 2;


					trishape[1][0] = (g[0] - BASE / 2);
					trishape[1][1] = (g[1] + SIZE);

					trishape[2][0] = (g[0] + BASE / 2);
					trishape[2][1] = (g[1] + SIZE);
					setColor();

				}
				if (trishape[i][1] < -0.5) //SOUTH
				{
					t = uidD(mt);
					while (t == S && t == SW && t == SE) t = uidD(mt);
					d = (dir)t;
					setdir();
					state = S;

					g[1] = -0.5 + SIZE;

					trishape[0][0] = g[0];
					trishape[0][1] = g[1] + SIZE * 2;


					trishape[1][0] = (g[0] - BASE / 2);
					trishape[1][1] = (g[1] - SIZE);

					trishape[2][0] = (g[0] + BASE / 2);
					trishape[2][1] = (g[1] - SIZE);
					setColor();

				}

			}
			
			else if (type == 0)
			{
				if (trishape[i][0] < -1.0 ) //WEST
				{
					t = uidD(mt);
					while (t == W && t == NW && t == SW) t = uidD(mt);
					d = (dir)t;
					setdir();
					state = W;

					 g[0] = -1.0 + SIZE;
					//if (trishape[i][0] < 0.5) g[0] = 0.5 + SIZE;
					trishape[0][0] = g[0] + SIZE * 2;
					trishape[0][1] = g[1];

					trishape[1][0] = (g[0] - SIZE);
					trishape[1][1] = (g[1] - BASE / 2);


					trishape[2][0] = (g[0] - SIZE);
					trishape[2][1] = (g[1] + BASE / 2);
					setColor();
					break;
				}
				else if (trishape[i][0] > 1.0 ) //EAST
				{
					t = uidD(mt);
					while (t == E && t == NE && t == SE) t = uidD(mt);
					d = (dir)t;
					setdir();
					state = E;

					g[0] = 1.0 - SIZE;
					//if (trishape[i][0] > -0.5)g[0] = -0.5 - SIZE;
					trishape[0][0] = g[0] - SIZE * 2;
					trishape[0][1] = g[1];


					trishape[1][0] = (g[0] + SIZE);
					trishape[1][1] = (g[1] - BASE / 2);


					trishape[2][0] = (g[0] + SIZE);
					trishape[2][1] = (g[1] + BASE / 2);
					setColor();
					break;
				}
				else if (trishape[i][1] > 1.0 ) //NORTH
				{
					t = uidD(mt);
					while (t == N && t == NW && t == NE) t = uidD(mt);
					d = (dir)t;
					setdir();
					state = N;

					g[1] = 1.0 - SIZE;
					//if (trishape[i][1] < -0.5)g[1] = -0.5 - SIZE;
					trishape[0][0] = g[0];
					trishape[0][1] = g[1] - SIZE * 2;


					trishape[1][0] = (g[0] - BASE / 2);
					trishape[1][1] = (g[1] + SIZE);

					trishape[2][0] = (g[0] + BASE / 2);
					trishape[2][1] = (g[1] + SIZE);
					setColor();
					break;
				}
				else if (trishape[i][1] < -1.0 ) //SOUTH
				{
					t = uidD(mt);
					while (t == S && t == SW && t == SE) t = uidD(mt);
					d = (dir)t;
					setdir();
					state = S;

					g[1] = -1.0 + SIZE;
					//if(trishape[i][1] < 0.5)g[1] = 0.5 + SIZE;

					trishape[0][0] = g[0];
					trishape[0][1] = g[1] + SIZE * 2;


					trishape[1][0] = (g[0] - BASE / 2);
					trishape[1][1] = (g[1] - SIZE);

					trishape[2][0] = (g[0] + BASE / 2);
					trishape[2][1] = (g[1] - SIZE);
					setColor();
					break;
				}
				
				else if (temp[i][1]< 0.5 &&temp[i][1] > -0.5)
				{
					if (temp[i][0] < -0.5 && temp[i][0] > -1.0)
					{
						if (trishape[i][0] > -0.5)
						{
							t = uidD(mt);
							while (t == E || t == NE || t == SE) t = uidD(mt);
							state = E;
							g[0] = -0.5 - SIZE;
							trishape[0][0] = g[0] - (SIZE * 2);
							trishape[0][1] = g[1];

							trishape[1][0] = (g[0] + SIZE);
							trishape[1][1] = (g[1] - BASE / 2);

							trishape[2][0] = (g[0] + SIZE);
							trishape[2][1] = (g[1] + BASE / 2);
							d = (dir)t;
							setdir();
							

							setColor();
						}

					}
				}
				else if (temp[i][1]< 1.0 &&temp[i][1] > 0.5)
				{
					if (temp[i][0] < 0.5 && temp[i][0] > -0.5)
					{
						if (trishape[i][1] < 0.5)
						{
							t = uidD(mt);
							while (t == S || t == SW || t == SE) t = uidD(mt);
							state = S;
							g[1] = 0.5 + SIZE;
							trishape[0][0] = g[0];
							trishape[0][1] = g[1] + SIZE * 2;


							trishape[1][0] = (g[0] - BASE / 2);
							trishape[1][1] = (g[1] - SIZE);

							trishape[2][0] = (g[0] + BASE / 2);
							trishape[2][1] = (g[1] - SIZE);
							d = (dir)t;
							setdir();
							setColor();
						}

					}
				}
				else if (temp[i][1]< -0.5 &&temp[i][1] > -1.0)
				{
					if (temp[i][0] < 0.5 && temp[i][0] > -0.5)
					{
						if (trishape[i][1] > -0.5)
						{
							t = uidD(mt);
							while (t == N || t == NE || t == NW) t = uidD(mt);
							state = N;
							g[1] = -0.5 - SIZE;
							trishape[0][0] = g[0];
							trishape[0][1] = g[1] - SIZE * 2;


							trishape[1][0] = (g[0] - BASE / 2);
							trishape[1][1] = (g[1] + SIZE);

							trishape[2][0] = (g[0] + BASE / 2);
							trishape[2][1] = (g[1] + SIZE);
							d = (dir)t;
							setdir();
							setColor();
						}

					}
				}
				if (temp[i][1]< 0.5 &&temp[i][1] > -0.5)
				{
					if (temp[i][0] < 1.0 && temp[i][0] > 0.5)
					{
						if (trishape[i][0] < 0.5)
						{
							t = uidD(mt);
							while (t == W || t == NW || t == SW) t = uidD(mt);
							state = W;
							g[0] = 0.5 + SIZE;
							trishape[0][0] = g[0] + SIZE * 2;
							trishape[0][1] = g[1];

							trishape[1][0] = (g[0] - SIZE);
							trishape[1][1] = (g[1] - BASE / 2);


							trishape[2][0] = (g[0] - SIZE);
							trishape[2][1] = (g[1] + BASE / 2);

							d = (dir)t;
							setdir();
							setColor();
						}

					}
				}

			}

		}

		

	}
	void update()
	{
		if (state == NONE)
		{
			trishape[0][0] = g[0];
			trishape[0][1] = g[1] + SIZE * 2;

			trishape[1][0] = (g[0] - BASE / 2);
			trishape[1][1] = (g[1] - SIZE);

			trishape[2][0] = (g[0] + BASE / 2);
			trishape[2][1] = (g[1] - SIZE);
		}
		if (state == W) //WEST
		{

			trishape[0][0] = g[0] + SIZE * 2;
			trishape[0][1] = g[1];

			trishape[1][0] = (g[0] - SIZE);
			trishape[1][1] = (g[1] - BASE / 2);


			trishape[2][0] = (g[0] - SIZE);
			trishape[2][1] = (g[1] + BASE / 2);

		}
		if (state == E) //EAST
		{


			trishape[0][0] = g[0] - SIZE * 2;
			trishape[0][1] = g[1];


			trishape[1][0] = (g[0] + SIZE);
			trishape[1][1] = (g[1] - BASE / 2);


			trishape[2][0] = (g[0] + SIZE);
			trishape[2][1] = (g[1] + BASE / 2);

		}
		if (state == N) //NORTH
		{

			trishape[0][0] = g[0];
			trishape[0][1] = g[1] - SIZE * 2;


			trishape[1][0] = (g[0] - BASE / 2);
			trishape[1][1] = (g[1] + SIZE);

			trishape[2][0] = (g[0] + BASE / 2);
			trishape[2][1] = (g[1] + SIZE);


		}
		if (state == S) //SOUTH
		{


			trishape[0][0] = g[0];
			trishape[0][1] = g[1] + SIZE * 2;


			trishape[1][0] = (g[0] - BASE / 2);
			trishape[1][1] = (g[1] - SIZE);

			trishape[2][0] = (g[0] + BASE / 2);
			trishape[2][1] = (g[1] - SIZE);

		}

	}

	void teleport(int key)
	{
		if (type == 0)
		{
			switch (key)
			{
			case 'w':
			case 'W':
				g[0] = 0.0f;
				g[1] = 0.6f;
				break;
			case 'a':
			case 'A':
				g[0] = -0.6f;
				g[1] = 0.0f;
				break;
			case 's':
			case 'S':
				g[0] = 0.0f;
				g[1] = -0.6f;
				break;
			case 'd':
			case 'D':
				g[0] = 0.6f;
				g[1] = 0.0f;
				break;
			}
			update();
		}
		
	}
};

triangle tri[COUNT] = {triangle(1),triangle(1),triangle(0),triangle(0),triangle(0),triangle(0) ,triangle(0),triangle(0),triangle(0),triangle(0) };
float inrect[4][2] = { {0.5,0.5},{-0.5,0.5},{-0.5,-0.5},{0.5,-0.5} };

GLint index = 0, time = 10, frame = 0;
GLboolean stop = 0;
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

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, tri[i].trishape, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		//색상
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

	for (int i = 0; i < 4; i++)
	{
		inrect[i][0] = 0.5;
		inrect[i][1] = 0.5;
	}

	InitShader();
	InitBuffer();
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard);


	glutMainLoop(); // 이벤트 처리 시작 }
}


GLvoid KeyBoard(unsigned char key, int x, int y)
{

	if (key == 'm' || key == 'M')
	{
		stop = 0;
		glutTimerFunc(time, TimerFunction, 1);
	}
	if (key == 't' || key == 'T')
	{
		stop = 1;
	}

	for (int i = 0; i < COUNT; i++) tri[i].teleport(key);

	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (stop == 1) return;

	for (int i = 0; i < COUNT; i++)
	{
		tri[i].move();
		tri[i].update();
		tri[i].collpse();
	}
	UpdateBuffer();

	

	glutPostRedisplay();
	glutTimerFunc(time, TimerFunction, 1);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
	
	glUseProgram(s_program);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.5, 0.5, -0.5, -0.5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int i = 0; i < COUNT; i++)
	{
		glBindVertexArray(vao[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//--- 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}