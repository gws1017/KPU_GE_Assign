#include <iostream>
#include <random>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <math.h>

using namespace std;

GLfloat X1 = 0, Y1 = 0, X2 = 0, Y2 = 0, speed = 18.0, sx , sy;
GLint click = 0, t = 50, dx = 1, dy = 1;
GLboolean stop = 0;
GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int, int, int, int);
GLvoid randomRGB();

int winWidth = 800, winHeight = 600;

void converDeviceXYOpenGLXY(int x, int y, float* ox, float* oy)
{
	int w = winWidth;
	int h = winHeight;
	*ox = (float)(x - (float)w / 2.0)*(float)(1.0 / (float)(w / 2.0));
	*oy = -(float)(y - (float)h / 2.0)*(float)(1.0 / (float)(h / 2.0));
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
GLvoid randomRGB() //0�̸� �簢������ 1�̸� ���
{
	float r, g, b;
	uniform_int_distribution<int> uid(0, 255);
	random_device rd;
	mt19937 mt(rd());

	r = uid(mt) * 1.0 / 255.0;
	g = uid(mt) * 1.0 / 255.0;
	b = uid(mt) * 1.0 / 255.0;
	glColor4f(r, g, b, 1.0);

}

GLvoid KeyBoard(unsigned char key, int x, int y)
{

	if (key == 'a' || key == 'A')
	{
		stop = 0;
		glutTimerFunc(t,TimerFunction,1);
	}
	if (key == 's' || key == 'S')
	{
		stop = 1;
	}

	glutPostRedisplay();
}
void collpse()
{
	if (X2 > 1.0)
	{
		X2 = 1.0;
		X1 = X2 - sx;
		dx = -1;
	}
	else if (X1 < -1.0)
	{
		X1 = -1.0;
		X2 = X1 + sx;
		dx = 1;
	}

	else if (Y2 > 1.0)
	{
		Y2 = 1.0;
		Y1 = Y2 - sy;
		dy = -1;
	}

	else if (Y1 < -1.0)
	{
		Y1 = -1.0;
		Y2 = Y1 + sy;
		dy = 1;
	}
}
void move()
{
	X1 += dx * speed / 800; // 800 : 1 = 5 : ?
	X2 += dx * speed / 800;
	Y1 += dy * speed / 600;
	Y2 += dy * speed / 600;
}
GLvoid Mouse(int button, int state, int x, int y)
{
	float ox, oy;

	converDeviceXYOpenGLXY(x, y, &ox, &oy);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		click = 1;
		X1 = ox - 0.1;
		X2 = ox + 0.1;
		Y1 = oy - 0.1;
		Y2 = oy + 0.1;

		sx = X2 - X1;
		sy = Y2 - Y1;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (stop == 1) return;
	move();
	collpse();
	glutPostRedisplay();
	glutTimerFunc(t, TimerFunction, 1);

}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����
	if (click == 1)
	{
		randomRGB();	
	}
	glRectf(X1, Y1, X2, Y2);
	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	click = 0;
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h); 
}