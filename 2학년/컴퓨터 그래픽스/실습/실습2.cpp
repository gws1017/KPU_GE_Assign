#include <iostream>
#include <random>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;

GLfloat X1 = -0.5, Y1 = -0.5, X2 = 0.5, Y2 = 0.5;
GLint click = 0;
GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int, int, int, int);
GLvoid randomRGB();

int winWidth = 800 , winHeight = 600;

void converDeviceXYOpenGLXY(int x, int y, float* ox, float* oy)
{
	int w = winWidth;
	int h = winHeight;
	*ox = (float)(x - (float)w / 2.0)*(float)(1.0 / (float)(w / 2.0));
	*oy = - (float)(y - (float)h / 2.0)*(float)(1.0 / (float)(h / 2.0));
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(winWidth, winHeight); // �������� ũ�� ����
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
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(KeyBoard);
	glutMouseFunc(Mouse);
	glutMainLoop(); // �̺�Ʈ ó�� ���� }
}
GLvoid randomRGB(int i) //0�̸� �簢������ 1�̸� ���
{
	float r, g, b;
	uniform_int_distribution<int> uid(0, 255);
	random_device rd;
	mt19937 mt(rd());

	r = uid(mt) * 1.0 / 255.0;
	g = uid(mt) * 1.0 / 255.0;
	b = uid(mt) * 1.0 / 255.0;
	if(i == 2 )glColor4f(r, g, b,1.0);
	if (i == 1)glClearColor(r, g, b, 1.0);
}

GLvoid KeyBoard(unsigned char key, int x, int y)
{
	
	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float ox, oy;

	converDeviceXYOpenGLXY(x, y, &ox, &oy);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if ((ox >= X1 && ox <= X2) && (oy >= Y1 && oy <= Y2))
		{
			click = 2;	
		}
		else
		{
			click = 1;
		}
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	if(click == 1) randomRGB(click);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����
	if(click == 2) randomRGB(click);
	glRectf(X1, Y1, X2, Y2);
	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	click = 0;
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}