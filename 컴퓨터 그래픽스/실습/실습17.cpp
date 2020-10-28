#include <iostream>
#include <random>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

//������ ũ��
#define WIDTH 500
#define HEIGHT 500

//����
#define COUNT 3 //0 �ٴ� 1 ��ü�Ʒ� 2��ü�� 3����տ��� 4 ����� ������
#define CP 8    //CUBE POINT
#define CI 36

//������ ũ��
#define CSZ1 0.2
#define CSZ2 0.15
#define BTSZ 1.0

glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 projTZ = glm::mat4(1.0f);
glm::mat4 projTX = glm::mat4(1.0f);
glm::mat4 projRY = glm::mat4(1.0f);

glm::mat4 view = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraPos2 = glm::vec3(0.0f, 1.0f, 8.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid ArrowKey(int, int, int);

GLint select = 0, t = 100, m = 0, a=0, delta_time = 0, delta_time2 = 0, state = 0;
GLboolean w = true, h = true, stop = true;
GLUquadric *qobj, *qobj2;

GLfloat cube_pos[CP][3] = {
	-CSZ1,0,CSZ1, //���ʾվƷ�
	CSZ1,0,CSZ1, // �����ʾվƷ�
	CSZ1,0,-CSZ1, //�����ʵھƷ�
	-CSZ1,0,-CSZ1, //���ʵھƷ�
	-CSZ1,CSZ1,CSZ1,
	CSZ1,CSZ1,CSZ1,
	CSZ1,CSZ1,-CSZ1,
	-CSZ1,CSZ1,-CSZ1
};

GLfloat cube_pos2[CP][3] = {
	-CSZ2,-0.1,CSZ2, //���ʾվƷ�
	CSZ2,-0.1,CSZ2, // �����ʾվƷ�
	CSZ2,-0.1,-CSZ2, //�����ʵھƷ�
	-CSZ2,-0.1,-CSZ2, //���ʵھƷ�
	-CSZ2,CSZ2,CSZ2,
	CSZ2,CSZ2,CSZ2,
	CSZ2,CSZ2,-CSZ2,
	-CSZ2,CSZ2,-CSZ2
};

GLint cube_index[CI] = {
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

/*GLint cube_index2[36] = {
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
};*/

GLfloat bottom_pos[4][3] = {
	-BTSZ,0.0,-BTSZ,
	-BTSZ,0.0,BTSZ,
	BTSZ,0.0,BTSZ,
	BTSZ,0.0,-BTSZ
};
GLint bottom_index[6] = {
	0,1,2,
	0,2,3
};
GLfloat grid_color[4][3] = { 0.0f, };

GLfloat cube_color1[CP][3] = {
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f
};

GLfloat cube_color2[CP][3] = {
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f
};

glm::mat4 World = glm::mat4(1.0f);
glm::mat4 R = glm::mat4(1.0f);
glm::mat4 T = glm::mat4(1.0f);

glm::mat4 Rx = glm::mat4(1.0f);
glm::mat4 Ry = glm::mat4(1.0f);

glm::mat4 Tx = glm::mat4(1.0f);
glm::mat4 Ty = glm::mat4(1.0f);

glm::mat4 C2 = glm::mat4(1.0f); //�ڽ� �ʱ���ġ ����
glm::mat4 C2Ty = glm::mat4(1.0f);

glm::mat4 CL = glm::mat4(1.0f); //�Ǹ��� �ʱ���ġ ����
glm::mat4 CLRX = glm::mat4(1.0f);
glm::mat4 CLTY = glm::mat4(1.0f);
glm::mat4 CLTX = glm::mat4(1.0f);
glm::mat4 CLTX2 = glm::mat4(1.0f);

glm::mat4 CR = glm::mat4(1.0f); // ũ���� ��ü ���
glm::mat4 CRTX = glm::mat4(1.0f);

glm::mat4 CRM = glm::mat4(1.0f); // ũ���� �ߴܺ� ���
glm::mat4 CRMRY = glm::mat4(1.0f);

glm::mat4 CRA = glm::mat4(1.0f); // ũ���� �� ���
glm::mat4 CRARX = glm::mat4(1.0f);
glm::mat4 CRATY = glm::mat4(1.0f);
glm::mat4 CRATZ = glm::mat4(1.0f);

glm::mat4 CRA2 = glm::mat4(1.0f); // ũ���� �� ���
glm::mat4 CRARX2 = glm::mat4(1.0f);
glm::mat4 CRATY2 = glm::mat4(1.0f);
glm::mat4 CRATZ2 = glm::mat4(1.0f);

glm::mat4 temp = glm::mat4(1.0f);
unsigned int modelLocation, projectionLocation, viewLocation;

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
GLuint vao[COUNT], vbo[COUNT][1], ebo[COUNT];
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
		if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(bottom_pos), bottom_pos, GL_STATIC_DRAW);
		else if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(cube_pos), cube_pos, GL_STATIC_DRAW);
		else if (i == 2) glBufferData(GL_ARRAY_BUFFER, sizeof(cube_pos2), cube_pos2, GL_STATIC_DRAW);
		//else if (i == 3) glBufferData(GL_ARRAY_BUFFER, sizeof(), , GL_STATIC_DRAW);
		//else if (i == 4) glBufferData(GL_ARRAY_BUFFER, sizeof(), , GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//����
		/*glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);
		if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(grid_color), grid_color, GL_STATIC_DRAW);
		else if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color1), cube_color1, GL_STATIC_DRAW);
		else if (i == 2) glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color2), cube_color2, GL_STATIC_DRAW);
		//else if (i == 3) glBufferData(GL_ARRAY_BUFFER, sizeof(), , GL_STATIC_DRAW);
		//else if (i == 4) glBufferData(GL_ARRAY_BUFFER, sizeof(), , GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);*/

		//EBO ����

		glGenBuffers(1, &ebo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		if (i == 0) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bottom_index), bottom_index, GL_STATIC_DRAW);
		if (i == 1 || i == 2) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index), cube_index, GL_STATIC_DRAW);
		//if (i == 2 || i == 3) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(), , GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);


	}


}

/*
void UpdateBuffer()
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

void DrawCylinder(GLUquadric* qobj)
{
	
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricOrientation(qobj, GLU_OUTSIDE);
	
	gluCylinder(qobj, 0.03, 0.03, 0.35, 20, 8);
}

void InitRT()
{
	

	R = glm::mat4(1.0f);

	Rx = glm::mat4(1.0f);
	Ry = glm::mat4(1.0f);


	C2Ty = glm::translate(C2Ty, glm::vec3(0.0f, CSZ2*2 , 0.0f));

	CLTY = glm::translate(CLTY, glm::vec3(0.0f, 0.35f+CSZ1+ CSZ2+ 0.1, 0.0f));
	CLRX = glm::rotate(CLRX,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
	CLTX = glm::translate(CLTX, glm::vec3(-0.1f, 0.0f, 0.0f));
	CLTX2 = glm::translate(CLTX2, glm::vec3(0.1f, 0.0f, 0.0f));
	//Rx = glm::rotate(Rx, glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0));
	Ry = glm::rotate(Ry, glm::radians(-20.0f), glm::vec3(0.0, 1.0, 0.0));

	R = Rx * Ry;
	

	
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(WIDTH, HEIGHT); // �������� ũ�� ����
	glutCreateWindow("Example"); // ������ ����(������ �̸�)
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
	InitRT();



	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(KeyBoard);
	glutSpecialFunc(ArrowKey);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}

void allinit()
{
	World = glm::mat4(1.0f);
	R = glm::mat4(1.0f);
	T = glm::mat4(1.0f);

	Rx = glm::mat4(1.0f);
	Ry = glm::mat4(1.0f);

	 Tx = glm::mat4(1.0f);
	 Ty = glm::mat4(1.0f);

	 C2 = glm::mat4(1.0f); //�ڽ� �ʱ���ġ ����
	 C2Ty = glm::mat4(1.0f);

	 CL = glm::mat4(1.0f); //�Ǹ��� �ʱ���ġ ����
	CLRX = glm::mat4(1.0f);
	CLTY = glm::mat4(1.0f);
	CLTX = glm::mat4(1.0f);
	CLTX2 = glm::mat4(1.0f);

	CR = glm::mat4(1.0f); // ũ���� ��ü ���
	CRTX = glm::mat4(1.0f);

	CRM = glm::mat4(1.0f); // ũ���� �ߴܺ� ���
	CRMRY = glm::mat4(1.0f);

	CRA = glm::mat4(1.0f); // ũ���� �� ���
	CRARX = glm::mat4(1.0f);
	CRATY = glm::mat4(1.0f);
	CRATZ = glm::mat4(1.0f);

	CRA2 = glm::mat4(1.0f); // ũ���� �� ���
	CRARX2 = glm::mat4(1.0f);
	CRATY2 = glm::mat4(1.0f);
	CRATZ2 = glm::mat4(1.0f);

	temp = glm::mat4(1.0f);
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

	if (key == '1') select = 0;
	if (key == '2') select = 1;



	if (key == 'w') {
		if(w == false)w = true;
		else w = false;
	}
	

	if (key == 'h') {
		if (!h)
		{
			h = true;


		}
		else
		{
			h = false;


		}
	}
	if (key == 'z') {
		projTZ = glm::translate(projTZ, glm::vec3(0.0f, 0.0f, 0.5f));
	}
	if (key == 'Z') {
		projTZ = glm::translate(projTZ, glm::vec3(0.0f, 0.0f, -0.5f));
	}
	if (key == 'x') {
		projTZ = glm::translate(projTZ, glm::vec3(0.5f, 0.0f, 0.0f));
	}
	if (key == 'X') {
		projTZ = glm::translate(projTZ, glm::vec3(-0.5f, 0.0f, 0.0f));
	}
	if (key == 'r') {
		projRY = glm::rotate(projRY,glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (key == 'R') {
		projRY = glm::rotate(projRY, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (key == 'b') CRTX = glm::translate(CRTX, glm::vec3(0.1f, 0.0f, 0.0f));
	if (key == 'B') CRTX = glm::translate(CRTX, glm::vec3(-0.1f, 0.0f, 0.0f));
	if ( key == 'm' || key == 'M' || key == 't' || key == 'T' || key == 'O')
	{
		if (stop == true)
		{
			stop = false;
			glutTimerFunc(t, TimerFunction, 1);
		}
		
		if (key == 'm') m = 1;
		if (key == 'M') m = -1;
		if (key == 't') a = 1;
		if (key == 'T') a = -1;
	}
	if (key == 's' || key == 'S')
	{
		stop = true;
	}
	if (key == 'c' || key == 'C')
	{
		select = 0, t = 100, m = 0, a = 0, delta_time = 0, delta_time2 = 0, state = 0;
		stop = true;
		allinit();
		InitRT();
	}
	if (key == 'q' || key == 'Q')
	{
		exit(0);
	}
	glutPostRedisplay();
}

void Rotate()
{
	CRMRY = glm::rotate(CRMRY, glm::radians(m*10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	CRARX = glm::rotate(CRARX, glm::radians(a*7.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	CRARX2 = glm::rotate(CRARX2, glm::radians(a*-7.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}
void Translate()
{
	CRATZ = glm::translate(CRATZ, glm::vec3(0.0f, 0.0f, a*0.035f));
	CRATY = glm::translate(CRATY, glm::vec3(0.0f, a*-0.02f, 0.0f));

	CRATZ2 = glm::translate(CRATZ2, glm::vec3(0.0f, 0.0f, a*-0.035f));
	CRATY2 = glm::translate(CRATY2, glm::vec3(0.0f, a*-0.02f, 0.0f));
}



GLvoid TimerFunction(int value)
{
	if (stop) return;

	if(m!= 0) delta_time++;
	if (a!= 0) delta_time2++;
	if (delta_time % 10 == 0)
	{
		delta_time = 0;
		m = 0;
		
	}

	if (a != 0)
	{
		if (delta_time2 % 10 == 0 && (state == 0))
		{
			a = -1;
			state = 1;

		}
		else if (delta_time2 % 10 == 0 && state == 1)
		{
			a = 1;
			state = 2;
		}
		else if (delta_time2 % 10 == 0 && state == 2)
		{
			a = -1;
			state = 3;
		}
		else if (delta_time2 % 10 == 0 && state == 3)
		{
			a = 1;
			state = 0;
		}
	}
	
	
	
	Rotate();
	Translate();

	glutPostRedisplay();
	glutTimerFunc(t, TimerFunction, 1);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����
	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glUseProgram(s_program);
	int vColorLocation = glGetUniformLocation(s_program, "outColor");

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	viewLocation = glGetUniformLocation(s_program, "viewTransform");

	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 50.0f);
	projection = projection * projTZ * projTX * projRY;
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	if (!w)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (h)glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	modelLocation = glGetUniformLocation(s_program, "modelTransform");


	//�����̰���

	
	World = R * T;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(World));

	CR = CRTX;

	C2 = C2Ty;

	CRM = CRMRY;

	CRA = CRATZ * CRATY  ;
	CRA2 = CRATZ2 * CRATY2;
	for (int i = 0; i < COUNT; i++)
	{
		glBindVertexArray(vao[i]);
		if(i == 0)glUniform4f(vColorLocation, 0.3f, 0.3f, 0.3f, 1.0); // �ٴ�
		if (i == 1)
		{
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(World * CR));
			glUniform4f(vColorLocation, 1.0f, 0.0f, 0.0f, 1.0);
		}
		if (i == 2) {
			glUniform4f(vColorLocation, 1.0f, 1.0f, 0.0f, 1.0);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(World * CR * CRM * C2 ));
		}
		glDrawElements(GL_TRIANGLES, CI, GL_UNSIGNED_INT, 0);
	}

	CL = CLTY * CLRX;
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr( CL));
	glUniform4f(vColorLocation, 0.0f, 1.0f, 1.0f, 1.0);

	if(state == 0 || state == 1)glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(World* CR * CRM * CRA * CL * CLTX * CRARX));
	else if(state ==2 || state ==3)glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(World* CR * CRM * CRA2 * CL * CLTX * CRARX2));
	DrawCylinder(qobj);
	if (state == 0 || state == 1)glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(World* CR * CRM * CRA2 * CL * CLTX2* CRARX2));
	else if(state == 2 || state == 3)glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(World* CR * CRM * CRA * CL * CLTX2* CRARX));
	DrawCylinder(qobj2);

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�

}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}