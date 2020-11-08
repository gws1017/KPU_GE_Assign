#include <iostream>
#include <random>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;

GLint time = 50 , stop = 0;
GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid randomRGB();

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0,0); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
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
	
	glutMainLoop(); // �̺�Ʈ ó�� ���� }
}

GLvoid randomRGB()
{
	float r, g, b;
	uniform_int_distribution<int> uid(0, 255);
	random_device rd;
	mt19937 mt(rd());

	r = uid(mt) * 1.0 / 255.0;
	g = uid(mt) * 1.0 / 255.0;
	b = uid(mt) * 1.0 / 255.0;
	glClearColor(r, g, b, 1.0);
}
GLvoid KeyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
	case 'R':
		glClearColor(1.0, 0.0, 0.0, 1.0);
		break;
	case 'g':
	case 'G':
		glClearColor(0.0, 1.0, 0.0, 1.0);
		break;
	case 'b':
	case 'B':
		glClearColor(0.0, 0.0, 1.0, 1.0);
		break;
	case 'a':
	case 'A':
		randomRGB();
		break;
	case 'w':
	case 'W':
		glClearColor(1.0, 1.0, 1.0, 1.0);
		break;
	case 'k':
	case 'K':
		glClearColor(0.0, 0.0, 0.0, 1.0);
		break;
	case 't':
	case 'T':
		stop = 0;
		glutTimerFunc(time, TimerFunction, 1);
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		stop = 1;
		break;
	case 'q':
	case 'Q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (stop == 1) return;
	randomRGB();
	if (stop == 0)
	{
		glutPostRedisplay();
		glutTimerFunc(time, TimerFunction, 1);
	}
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����
	//--- �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}