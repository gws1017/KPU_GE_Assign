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
#define COUNT 2
#define GP 200 
#define SIZE 0.1
#define SIZEB 0.3
#define HZ 8.0f
#define PI 3.141592f

GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid ArrowKey(int, int, int);
void Rotate();

GLfloat ds = 1.0;
GLint select = 0,pycu = 1,  dx = 1, dy = 1, pc = 0, t = 10,tsin = 0, sc =0;
GLboolean  h = false, stop = true, dm = false, Tr = false, Ts = false,Tro = false;


GLfloat grid_pos[4][3] = {
	-1.0,0.0,0.0,
	1.0,0.0,0.0,
	0.0,-1.0,0.0,
	0.0,1.0,0.0
};

GLfloat sin_pos[GP][3] = {0.0f, };
GLfloat spring_pos[GP][3] = { 0.0f, };
GLfloat zig_pos[GP][3] = { 0.0f, };

GLfloat rec_pos[8][3] = { 0.0f, };
GLfloat tri_pos[5][3] = { 0.0f, };

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

GLfloat grid_color[4][3] = { 0.0f, };
GLfloat bazier_color[GP][3] = { 0.0f, };
GLfloat cube_color[8][3];
GLfloat pyramid_color[5][3];


glm::mat4 R = glm::mat4(1.0f);
glm::mat4 T = glm::mat4(1.0f);
glm::mat4 S = glm::mat4(1.0f);
glm::mat4 Rx = glm::mat4(1.0f);
glm::mat4 Ry = glm::mat4(1.0f);
glm::mat4 Tx = glm::mat4(1.0f);
glm::mat4 Ty = glm::mat4(1.0f);
glm::mat4 Sx = glm::mat4(1.0f);
glm::mat4 Sy = glm::mat4(1.0f);

unsigned int modelLocation;

void init_sin()
{
	
	for (int i = -90; i < 90; i++, pc++)
	{
		sin_pos[pc][0] = i / 90.0f;
		sin_pos[pc][1] = sin(glm::radians((float)i *HZ/2.0f))*SIZEB;
	}

}
void init_spring()
{

	pc = 0;
	for (int i = -90; i <= 90; i++, pc++)
	{
		spring_pos[pc][0] = cos(glm::radians((i-90)*HZ))*SIZEB + i/90.0f;
		spring_pos[pc][1] = sin(glm::radians((i - 90)*HZ))*SIZEB ;
	}

}
void init_zig()
{
	pc = 0;
	double d = 1;
	double m = 1;
	for (int i = -90; i <= 90; i++, pc++)
	{
		zig_pos[pc][0] = i / 90.0f;
		if (zig_pos[pc][0] >= -1 && zig_pos[pc][0] <= -0.75)
		{
			zig_pos[pc][1] = 4 * (zig_pos[pc][0] + 1)*SIZEB;
		}
		if (zig_pos[pc][0] <= -0.25 && zig_pos[pc][0] >= -0.75)
		{
			zig_pos[pc][1] = -4 * (zig_pos[pc][0] + 0.5)*SIZEB;
		}
		if (zig_pos[pc][0] >= -0.25 && zig_pos[pc][0] <= 0.25)
		{
			zig_pos[pc][1] = 4 * (zig_pos[pc][0])*SIZEB;
		}
		if (zig_pos[pc][0] >= 0.25 && zig_pos[pc][0] <= 0.75)
		{
			zig_pos[pc][1] = -4 * (zig_pos[pc][0] - 0.5)*SIZEB;
		}
		if (zig_pos[pc][0] >= 0.75 && zig_pos[pc][0] <= 1)
		{
			zig_pos[pc][1] = 4 * (zig_pos[pc][0] - 1)*SIZEB;
		}
	
	}
}


void init_rec2()
{
	rec_pos[0][0] = -SIZE;
	rec_pos[0][1] = -SIZE;
	rec_pos[0][2] = 0;
	//SIZE;
	rec_pos[1][0] = SIZE;
	rec_pos[1][1] = -SIZE;
	rec_pos[1][2] = 0;
	//SIZE
	rec_pos[2][0] = SIZE;
	rec_pos[2][1] = SIZE;
	rec_pos[2][2] = 0;
	// -SIZE,
	rec_pos[3][0] = -SIZE;
	rec_pos[3][1] = SIZE;
	rec_pos[3][2] = 0;
}

void init_rec3()
{
	rec_pos[0][0] = -SIZE;
	rec_pos[0][1] = -SIZE;
	rec_pos[0][2] = SIZE;
		//SIZE;
	rec_pos[1][0] = SIZE;
	rec_pos[1][1] = -SIZE;
	rec_pos[1][2] = SIZE;
		//SIZE
	rec_pos[2][0] = SIZE; 
	rec_pos[2][1] = -SIZE;
	rec_pos[2][2] = -SIZE;
		// -SIZE,
	rec_pos[3][0] = -SIZE;
	rec_pos[3][1] = -SIZE;
	rec_pos[3][2] = -SIZE;
	// -SIZE,
	rec_pos[4][0] = -SIZE;
	rec_pos[4][1] = SIZE;
	rec_pos[4][2] = SIZE;
		// SIZE,
	rec_pos[5][0] = SIZE;
	rec_pos[5][1] = SIZE;
	rec_pos[5][2] = SIZE;
		// SIZE,
	rec_pos[6][0] = SIZE;
	rec_pos[6][1] = SIZE;
	rec_pos[6][2] = -SIZE;
		// -SIZE,
	rec_pos[7][0] = -SIZE;
	rec_pos[7][1] = SIZE;
	rec_pos[7][2] = -SIZE;
		// -SIZE
}

void init_tri2()
{
	tri_pos[0][0] = -SIZE;
	tri_pos[0][1] = -SIZE;
	tri_pos[0][2] = 0.0f;
	//SIZE;
	tri_pos[1][0] = SIZE;
	tri_pos[1][1] = -SIZE;
	tri_pos[1][2] = 0.0f;
	
	tri_pos[2][0] = 0.0f;
	tri_pos[2][1] = SIZE;
	tri_pos[2][2] = 0.0f;

}

void init_tri3()
{
	tri_pos[0][0] = -SIZE;
	tri_pos[0][1] = -SIZE;
	tri_pos[0][2] = SIZE;
	//SIZE;
	tri_pos[1][0] = SIZE;
	tri_pos[1][1] = -SIZE;
	tri_pos[1][2] = SIZE;
	//SIZE
	tri_pos[2][0] = SIZE;
	tri_pos[2][1] = -SIZE;
	tri_pos[2][2] = -SIZE;
	// -SIZE,
	tri_pos[3][0] = -SIZE;
	tri_pos[3][1] = -SIZE;
	tri_pos[3][2] = -SIZE;
	// -SIZE,
	tri_pos[4][0] = 0.0f;
	tri_pos[4][1] = SIZE;
	tri_pos[4][2] = 0.0f;		
	
}





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
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)cube_color[i][j] = randomRGB();
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 3; j++)pyramid_color[i][j] = randomRGB();

	for (int i = 0; i < COUNT; i++)
	{
		glGenVertexArrays(1, &vao[i]);

		glBindVertexArray(vao[i]);
		glGenBuffers(2, vbo[i]);

		//위치
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);
		if(i == 0)glBufferData(GL_ARRAY_BUFFER, sizeof(sin_pos), sin_pos, GL_STATIC_DRAW);
		if (i == 1)glBufferData(GL_ARRAY_BUFFER, sizeof(rec_pos), rec_pos, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//색상
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);
		if(i == 0 )glBufferData(GL_ARRAY_BUFFER, sizeof(bazier_color),bazier_color, GL_STATIC_DRAW);
		if (i == 1)glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_color), pyramid_color, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		//EBO 설정
		/*if (i == 1)
		{
			glGenBuffers(1, &ebo[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_index), pyramid_index, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
		}*/
	


	}

}


void UpdateBuffer()
{
	for (int i = 0; i < COUNT; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);

		if (i == 0)
		{
			if (select == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(sin_pos), sin_pos, GL_STATIC_DRAW);
			else if (select == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(spring_pos), spring_pos, GL_STATIC_DRAW);
			else if (select == 2) glBufferData(GL_ARRAY_BUFFER, sizeof(zig_pos), zig_pos, GL_STATIC_DRAW);
		}
		if (i == 1)
		{
			if (pycu == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(rec_pos), rec_pos, GL_STATIC_DRAW);
			else if (pycu == 2) glBufferData(GL_ARRAY_BUFFER, sizeof(tri_pos), tri_pos, GL_STATIC_DRAW);
		}

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		if (i == 1 && dm == true)
		{
			glGenBuffers(1, &ebo[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
			if (pycu == 1) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index), cube_index, GL_STATIC_DRAW);
			else if (pycu == 2)glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_index), pyramid_index, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
		}
	}
}

void InitRotate()
{
	R = glm::mat4(1.0f);

	Rx = glm::mat4(1.0f);
	Ry = glm::mat4(1.0f);

	glUseProgram(s_program);
	modelLocation = glGetUniformLocation(s_program, "modelTransform");

	Rx = glm::rotate(Rx, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	Ry = glm::rotate(Ry, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));

	R = Rx * Ry;

	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R));
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(WIDTH, HEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Graph:1,2,3 rec:u tri:p Dimension: d Ani:t/s Rotate : r/R Init: c"); // 윈도우 생성(윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	init_sin();
	init_spring();
	init_zig();
	init_rec2();
	init_tri2();

	InitShader();
	InitBuffer();
	



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
	if (key == '1' || key == '2' || key == '3')
	{
		if (key == '1') select = 0;
		if (key == '2') select = 1;
		if (key == '3') select = 2;
		UpdateBuffer();
	}
	
	if (key == 'u')
	{
		pycu = 1;
		UpdateBuffer();
	}
	if (key == 'p')
	{
		pycu = 2;
		UpdateBuffer();
	}
	if (key == 'd')
	{
		if (dm)
		{
			dm = false;
			init_rec2();
			init_tri2();
		}
		else
		{
			dm = true;
			init_rec3();
			init_tri3();
		}
		UpdateBuffer();
	}

	if (key == 't' || key == 'T') {
		if (Tr) Tr = false;
		else
		{
			stop = false;
			if (Ts == false && Tr == false)glutTimerFunc(t, TimerFunction, 1);

			Tr = true;	
		}

	}
	if (key == 'r' || key == 'R')
	{
		if (key == 'r') dy = -1;
		if (key == 'R') dy = 1;
		Rotate();
		
	}
	
	if (key == 's' || key == 'S')
	{
		if (Ts) Ts = false;
		else
		{
			stop = false;
			if (Ts == false && Tr == false) glutTimerFunc(t, TimerFunction, 1);
			Ts = true;
			
			
		}
	
	}
	if (key == 'c' || key == 'C')
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> uid(0, 2);
		stop = true;
		dm = false;
		Ts = false;
		Tr = false;
		Tro = false;
		sc = 0;
		tsin = 0;
		R = glm::mat4(1.0f);
		Ry = glm::mat4(1.0f);
		T = glm::mat4(1.0f);
		S = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T*R*S));
		init_rec2();
		init_tri2();
		pycu = 1;
		select = uid(mt);
		UpdateBuffer();
	}
	glutPostRedisplay();
}

void Translate()
{
	Tx = glm::mat4(1.0f);
	Ty = glm::mat4(1.0f);
	if (select == 0)
	{
		if (tsin / 90.0f > 1.0f) tsin = -90;

		

		Tx = glm::translate(Tx, glm::vec3(tsin/90.0f, 0.0f, 0.0f));
		Ty = glm::translate(Ty, glm::vec3(0.0, sin(glm::radians((float)tsin *HZ / 2.0f))*SIZEB, 0.0));
		tsin++;
	}
	
	if (select == 1)
	{
		if (tsin / 90.0f > 1.0f) tsin = -90;

		Tx = glm::translate(Tx, glm::vec3(cos(glm::radians((tsin - 90)*HZ))*SIZEB + tsin / 90.0f , 0.0f, 0.0f));
		Ty = glm::translate(Ty, glm::vec3(0.0, sin(glm::radians((tsin - 90)*HZ))*SIZEB, 0.0));
		tsin++;
	}
	if (select == 2)
	{
		if (tsin / 90.0f > 1.0f) tsin = -90;

		Tx = glm::translate(Tx, glm::vec3(tsin / 90.0f, 0.0f, 0.0f));

		if (tsin / 90.0f >= -1 && tsin / 90.0f <= -0.75)
		{
			Ty = glm::translate(Ty, glm::vec3(0.0, 4 * (tsin / 90.0f + 1)*SIZEB, 0.0));
		}
		if (tsin / 90.0f <= -0.25 && tsin / 90.0f >= -0.75)
		{
			Ty = glm::translate(Ty, glm::vec3(0.0, -4 * (tsin / 90.0f + 0.5)*SIZEB, 0.0)); 
		}
		if (tsin / 90.0f >= -0.25 && tsin / 90.0f <= 0.25)
		{
			Ty = glm::translate(Ty, glm::vec3(0.0, 4 * (tsin / 90.0f)*SIZEB, 0.0));
		}
		if (tsin / 90.0f >= 0.25 && tsin / 90.0f <= 0.75)
		{
			Ty = glm::translate(Ty, glm::vec3(0.0, -4 * (tsin / 90.0f - 0.5)*SIZEB, 0.0));
		}
		if (tsin / 90.0f >= 0.75 && tsin / 90.0f <= 1)
		{
			Ty = glm::translate(Ty, glm::vec3(0.0, 4 * (tsin / 90.0f - 1)*SIZEB, 0.0));
		}
		
		
		tsin++;
	}

	T = Tx * Ty;
	

}

void Scale(float s)
{
	if (ds > 1.5) dx *= -1;
	if (ds < 0.5) dx *= -1;
	ds += dx * 0.1;
	
	
	S = glm::mat4(1.0f);
	S = glm::scale(S, glm::vec3(s, s, 1.0));
	
	
}
	


void Rotate()
{

	
	Ry = glm::rotate(Ry, glm::radians(dy * 10.0f), glm::vec3(0.0, 1.0, 0.0));
	R = Ry;
	

}

GLvoid TimerFunction(int value)
{
	if (stop) return;

	if (Tr == true) Translate();


	sc++;
	
	if (sc % 2 == 0)// 5 10 15 20
	{
		
		if(Ts == true) Scale(ds);
	}
	
	if (sc > 30) sc = 0;


	

	UpdateBuffer();
	glutPostRedisplay();
	glutTimerFunc(t, TimerFunction, 1);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.

	DrawCL();

    glUseProgram(s_program);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R));
	
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINE_STRIP, 0, pc-1);
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(R*T*S));
	glBindVertexArray(vao[1]);
	if (pycu == 1)
	{
		if(!dm)glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		else glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		
	}
	if (pycu == 2)
	{
		if(!dm)glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
		else glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	}
	glutSwapBuffers(); // 화면에 출력하기

}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}