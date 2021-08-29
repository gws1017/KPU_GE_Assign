#include <iostream>
#include <random>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;

GLint time = 50 , stop = 0;
GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid randomRGB();

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0,0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
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
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard);
	
	glutMainLoop(); // 이벤트 처리 시작 }
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

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}