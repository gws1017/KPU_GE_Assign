#include <iostream>
#include <random>
#include <gl/glew.h> // 필요한 헤더파일 include
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

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(winWidth, winHeight); // 윈도우의 크기 지정
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
	glutMouseFunc(Mouse);
	glutMainLoop(); // 이벤트 처리 시작 }
}
GLvoid randomRGB(int i) //0이면 사각형내부 1이면 배경
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

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	if(click == 1) randomRGB(click);
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
	if(click == 2) randomRGB(click);
	glRectf(X1, Y1, X2, Y2);
	//--- 그리기 관련 부분이 여기에 포함된다.
	click = 0;
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}