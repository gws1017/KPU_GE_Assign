#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include "stb_image.h"
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm/ext.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TRI_MAX 4			//�ִ� �ﰢ���� ����
#define TRI_POINT 3			//�ﰢ�� ���� ����
#define TRI_ISO 0.0735		//�̵�ﰢ���� ������ִ� �� (���ﰢ������ �� ���� �󸶸� ���Ұ���)
#define TRI_LEN 0.2			//�ﰢ�� ����
#define TRI_IN_MAX 0		//�ȿ��� ƨ��� �ﰢ������

using namespace std;
struct linemanager {
	GLfloat LShape[2][3] = {
	{0.0,1.0,0.0},
	{0.0,-1.0,0.0}
	};// = {//�ﰢ�� ������ ��ǥ��

	GLfloat colors[2][3] = { // �ﰢ�� ������ ����
 {1.0, 0.0, 0.0},
 {1.0,0.0, 0.0} };

};
class testmen {
public:
	unsigned int VBO, VAO;
	float vertexData[48] = {
		//--- ��ġ //--- �븻 //--- �ؽ�ó ��ǥ
		-0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0, 0.0, 0.0,
		0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0, 1.0, 0.0,
		0.5f, 0.5f, 0.5f, 0.0, 0.0, 1.0, 1.0, 1.0,
		0.5f, 0.5f, 0.5f, 0.0, 0.0, 1.0, 1.0, 1.0,
		-0.5f, 0.5f, 0.5f, 0.0, 0.0, 1.0, 0.0, 1.0,
		-0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0, 0.0, 0.0 };
	testmen() {};
	void initBuffer() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- ��ġ �Ӽ�
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- �븻�� �Ӽ�
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- �ؽ�ó ��ǥ �Ӽ�
		glEnableVertexAttribArray(2);
	}

};
class trimanager {
public:
	GLfloat triShape[TRI_POINT][3] = {
	{-0.75,0.5,0.0},
	{-0.625,0.55,0.0},
	{-0.5,0.6,0.0}
	};// = {//�ﰢ�� ������ ��ǥ��
	GLfloat normal[3][3] = {
	{0.0f,1.0f,0.0f},
	{0.0f,1.0f,0.0f},
	{0.0f,1.0f,0.0f},
	};
	GLfloat texture[3][2] = {

	};
	GLfloat colors[TRI_POINT][3] = { // �ﰢ�� ������ ����
 {0.0, 1.0, 0.0},
 {0.0, 0.0, 1.0},
 {0.0, 1.0, 0.0} };

	GLfloat dx = 0.005, dy = 0.005;	//default = 0.005
	trimanager() {};
	void changeCoord(GLfloat x, GLfloat y, GLfloat z, int i)
	{
		triShape[i][0] = x;
		triShape[i][1] = y;
		triShape[i][2] = z;
	}

};

class CubeManager {
public:
	GLuint vao[6], vbo[6][3];	//vao,vbo vbo���� ������ �����븻 �����ؽ��� ��������

	GLfloat CShape[36][3] = {
		{-0.5,-0.5,-0.5},	//0							
		{-0.5,-0.5,0.5},	//1							
		{-0.5,0.5,-0.5},	//2  012�ﰢ�� ���ʸ�
		{-0.5,-0.5,0.5},	//1							
		{-0.5,0.5,0.5},		//3							
		{-0.5,0.5,-0.5},	//2  132�ﰢ�� ���ʸ�		  	

		{0.5,-0.5,0.5},		//5
		{0.5,0.5,-0.5},		//6		567�����ʸ�
		{0.5,0.5,0.5},		//7
		{0.5,-0.5,-0.5},	//4
		{0.5,0.5,-0.5},		//6		465�����ʸ�
		{0.5,-0.5,0.5},		//5

		{-0.5,0.5,-0.5},	//2  264 �޸�
		{0.5,0.5,-0.5},		//6
		{0.5,-0.5,-0.5},	//4
		{-0.5,-0.5,-0.5},	//0
		{-0.5,0.5,-0.5},	//2  024 �޸�
		{0.5,-0.5,-0.5},	//4

		{-0.5,-0.5,-0.5},	//0
		{0.5,-0.5,-0.5},	//4	041 �Ʒ���
		{-0.5,-0.5,0.5},	//1
		{-0.5,-0.5,0.5},	//1
		{0.5,-0.5,-0.5},	//4	145 �Ʒ���
		{0.5,-0.5,0.5},		//5

		{-0.5,-0.5,0.5},	//1
		{0.5,-0.5,0.5},		//5	153�ո�
		{-0.5,0.5,0.5},		//3
		{-0.5,0.5,0.5},		//3
		{0.5,-0.5,0.5},		//5	357�ո�
		{0.5,0.5,0.5},		//7

		{-0.5,0.5,-0.5},	//2  
		{-0.5,0.5,0.5},		//3	236 ����
		{0.5,0.5,-0.5},		//6
		{-0.5,0.5,0.5},		//3
		{0.5,0.5,0.5},		//7	376����
		{0.5,0.5,-0.5},		//6

		//{-0.5,-0.5,-0.5},	//0
		//{-0.5,-0.5,0.5},	//1
		//{-0.5,0.5,-0.5},	//2  012�ﰢ�� ���ʸ�
		//{-0.5,0.5,0.5},		//3
		//{0.5,-0.5,-0.5},	//4
		//{0.5,-0.5,0.5},		//5
		//{0.5,0.5,-0.5},		//6
		//{0.5,0.5,0.5},		//7
	};
	GLuint index[36] = {
		0,1,2,
		1,3,2,
		2,3,6,
		3,7,6,
		5,6,7,
		4,6,5,
		2,6,4,
		0,2,4,
		0,4,1,
		1,4,5,
		1,5,3,
		3,5,7


	};
	GLfloat normal[36][3]{
	{-1.0, 0.0, 0.0},	//0							
	{ -1.0,0.0,0.0 },	//1							
	{ -1.0,0.0,0.0 },	//2  012�ﰢ�� ���ʸ�
	{ -1.0,0.0,0.0 },	//1							
	{ -1.0,0.0,0.0 },	//3							
	{ -1.0,0.0,0.0 },	//2  132�ﰢ�� ���ʸ�	

	{ 1.0,0.0,0.0 },		//5
	{ 1.0,0.0,0.0 },		//6		567�����ʸ�
	{ 1.0,0.0,0.0 },		//7
	{ 1.0,0.0,0.0 },		//4
	{ 1.0,0.0,0.0 },		//6		465�����ʸ�
	{ 1.0,0.0,0.0 },		//5

	{ 0.0,0.0,-1.0 },		//2  264 �޸�
	{ 0.0,0.0,-1.0 },		//6
	{ 0.0,0.0,-1.0 },		//4
	{ 0.0,0.0,-1.0 },		//0
	{ 0.0,0.0,-1.0 },		//2  024 �޸�
	{ 0.0,0.0,-1.0 },		//4

	{ 0.0,-1.0,0.0 },		//0
	{ 0.0,-1.0,0.0 },		//4	041 �Ʒ���
	{ 0.0,-1.0,0.0 },		//1
	{ 0.0,-1.0,0.0 },		//1
	{ 0.0,-1.0,0.0 },		//4	145 �Ʒ���
	{ 0.0,-1.0,0.0 },		//5

	{ 0.0,0.0,1.0 },		//1
	{ 0.0,0.0,1.0 },		//5	153�ո�
	{ 0.0,0.0,1.0 },		//3p
	{ 0.0,0.0,1.0 },		//3
	{ 0.0,0.0,1.0 },		//5	357�ո�
	{ 0.0,0.0,1.0 },		//7

	{ 0.0,1.0,0.0 },		//2  
	{ 0.0,1.0,0.0 },		//3	236 ����
	{ 0.0,1.0,0.0 },		//6
	{ 0.0,1.0,0.0 },		//3
	{ 0.0,1.0,0.0 },		//7	376����
	{ 0.0,1.0,0.0 },		//6
	};
	GLfloat texture[36][2] = {
	{ 0.0,0.0},	//0							
	{ 1.0,0.0},	//1							
	{ 0.0,1.0},	//2  012�ﰢ�� ���ʸ�
	{ 1.0,0.0},	//1							
	{ 1.0,1.0},	//3							
	{ 0.0,1.0},	//2  132�ﰢ�� ���ʸ�	

	{ 0.0,0.0},		//5
	{ 1.0,1.0},		//6		567�����ʸ�
	{ 1.0,0.0},		//7
	{ 0.0,1.0},		//4
	{ 1.0,1.0},		//6		465�����ʸ�
	{ 0.0,0.0},		//5

	{ 1.0,1.0 },		//2  264 �޸�
	{ 1.0,0.0 },		//6
	{ 0.0,0.0 },		//4
	{ 0.0,1.0 },		//0
	{ 1.0,1.0 },		//2  024 �޸�
	{ 0.0,0.0 },		//4

	{ 0.0,0.0},		//0
	{ 1.0,0.0},		//4	041 �Ʒ���
	{ 0.0,1.0},		//1
	{ 0.0,1.0},		//1
	{ 1.0,0.0},		//4	145 �Ʒ���
	{ 1.0,1.0},		//5

	{ 0.0,0.0},		//1
	{ 1.0,0.0},		//5	153�ո�
	{ 0.0,1.0},		//3
	{ 0.0,1.0},		//3
	{ 1.0,0.0},		//5	357�ո�
	{ 1.0,1.0},		//7

	{ 0.0,1.0},		//2  
	{ 0.0,0.0},		//3	236 ����
	{ 1.0,1.0},		//6
	{ 0.0,0.0},		//3
	{ 1.0,0.0},		//7	376����
	{ 1.0,1.0},		//6

	};

	CubeManager() { };
	void initCubeBuffer()
	{
		for (int i = 0; i < 6; ++i)
		{
			glGenVertexArrays(1, &vao[i]); //--- VAO �� �����ϰ� �Ҵ��ϱ�
			glBindVertexArray(vao[i]); //--- VAO�� ���ε��ϱ�
			glGenBuffers(3, vbo[i]); //--- 3���� VBO�� �����ϰ� �Ҵ��ϱ�

			//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);
			// ���� triShape ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
			// triShape �迭�� ������: 9 * float
			glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(GLfloat), &CShape[i * 6], GL_STATIC_DRAW);
			// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			// attribute �ε��� 0���� ��밡���ϰ� ��
			glEnableVertexAttribArray(0);
			//---EBO���
			//

			//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);
			// ���� colors���� ���ؽ� ������ �����Ѵ�.
			// colors �迭�� ������: 3 * TRI_POINT ��ŭ
			glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(GLfloat), &normal[i * 6], GL_STATIC_DRAW);
			// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			// attribute �ε��� 1���� ��� �����ϰ� ��.
			glEnableVertexAttribArray(1);


			//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][2]);
			// �ؽ��Ŀ��� �� ������
			// 
			glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(GLfloat), &texture[i * 6], GL_STATIC_DRAW);
			// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float  3����ŭ ����� �дµ� ���� 2���� ��������
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			// attribute �ε��� 1���� ��� �����ϰ� ��.
			glEnableVertexAttribArray(2);
		}
	}
};
//
//class HalfDiamondManager {
//public:
//	trimanager face[5];
//
//	void init()
//	{
//		face[0].changeCoord(0.0, 0.5, 0.0, 0);
//		face[0].changeCoord(0.5, -0.5, -0.5, 1);
//		face[0].changeCoord(-0.5, -0.5, -0.5, 2);
//
//	}
//
//	GLfloat HDShape[18][3] = {
//		{0.0,0.5,0.0},			//0
//		{0.5,-0.5,-0.5},		//1		�޸�
//		{-0.5,-0.5,-0.5},		//2
//		
//		{0.0,0.5,0.0},			//0
//		{-0.5,-0.5,-0.5},		//2		���ʸ�
//		{-0.5,-0.5,0.5},		//3
//
//		{0.0,0.5,0.0},			//0
//		{-0.5,-0.5,0.5},		//3		�ո�
//		{0.5,-0.5,0.5},			//4
//
//		{0.0,0.5,0.0},			//0
//		{0.5,-0.5,0.5},			//4		�����U��
//		{0.5,-0.5,-0.5},		//1
//
//		{0.5,-0.5,-0.5},		//1
//		{0.5,-0.5,0.5},			//4		�Ʒ��簢�� �����ʾƷ�������
//		{-0.5,-0.5,0.5},		//3
//
//		{0.5,-0.5,-0.5},		//1
//		{-0.5,-0.5,0.5},		//3		���簢�� ������������
//		{-0.5,-0.5,-0.5},		//2
//
//		//{0.0,0.5,0.0},			//0
//		//{0.5,-0.5,-0.5},		//1		�޸�
//		//{-0.5,-0.5,-0.5},		//2								
//		//{-0.5,-0.5,0.5},		//3
//		//{0.5,-0.5,0.5}			//4
//	};
//	GLuint index[18] = {
//		0,1,2,
//		0,2,3,
//		0,3,4,
//		0,4,1,
//		1,4,3,
//		1,3,2
//	};
//	GLfloat normal[18][3] = {
//
//	};
//	GLfloat colors[5][3] = {
//	 {1.0, 0.0, 0.0},
//	{0.0, 1.0, 0.0},
//	{0.0, 0.0, 1.0},
//	{1.0, 1.0, 0.0},
//	{0.0, 1.0, 1.0}
//	};
//	GLfloat texture[5][3] = {
//
//	};
//};


class HalfDiamondManager {
public:
	GLuint EBO;
	vector<GLfloat> vertex;
	vector<GLfloat> normal;
	vector<GLfloat> texture;
	GLuint vao[8], vbo[8][3];	//vao,vbo vbo���� ������ �����븻 �����ؽ��� ��������

	HalfDiamondManager() { };
	//--ebo

	void initHalfDiamondBuffer()
	{
		for (int i = 0; i < 8; ++i)
		{
			glGenVertexArrays(1, &vao[i]); //--- VAO �� �����ϰ� �Ҵ��ϱ�
			glBindVertexArray(vao[i]); //--- VAO�� ���ε��ϱ�
			glGenBuffers(3, vbo[i]); //--- 3���� VBO�� �����ϰ� �Ҵ��ϱ�

			//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);
			// ���� triShape ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
			// triShape �迭�� ������: 9 * float
			glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), &vertex[i * 9], GL_STATIC_DRAW);
			// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			// attribute �ε��� 0���� ��밡���ϰ� ��
			glEnableVertexAttribArray(0);
			//---EBO���
			//

			//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);
			// ���� colors���� ���ؽ� ������ �����Ѵ�.
			// colors �迭�� ������: 3 * TRI_POINT ��ŭ
			glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), &normal[i * 9], GL_STATIC_DRAW);
			// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			// attribute �ε��� 1���� ��� �����ϰ� ��.
			glEnableVertexAttribArray(1);

			//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][2]);
			// ���� colors���� ���ؽ� ������ �����Ѵ�.
			// colors �迭�� ������: 3 * TRI_POINT ��ŭ
			glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), &texture[i * 6], GL_STATIC_DRAW);
			// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			// attribute �ε��� 1���� ��� �����ϰ� ��.
			glEnableVertexAttribArray(2);
		}
	}
	void Draw()
	{
		glBindVertexArray(vao[0]);
		glDrawArrays(GL_TRIANGLES, 0, vertex.size());
	}
};


struct RectManager {
	GLuint flag = 0;
	GLfloat oldx = 0, oldy = 0;
	GLuint index[6] = {
	0,1,3,
	1,2,3
	};
	GLfloat RShape[4][3] = {
{0.5,0.5,0.0},
{-0.5,0.5,0.0},
{-0.5,-0.5,0.0},
{0.5,-0.5,0.0} };// = {//�簢�� ������ ��ǥ��

	GLfloat colors[4][3] = { // �ﰢ�� ������ ����
 {0.0, 0.0, 7.0},
 {0.23, 1.0, 0.0},
 {0.0, 0.4, 0.0},
	{1.0,1.0,7.0} };
};

struct PentaManager {
	GLfloat PShape[5][3] = {
{0.75,-0.5,0.0},
{0.5,-0.5,0.0},
	{0.5,-0.75,0.0},
{0.6,-0.80,0.0},
	{0.75,-0.6,0.0} };// = {//������ ������ ��ǥ��

	GLfloat colors[5][3] = { // ������ ������ ����
 {1.0, 0.0, 1.0},
 {1.0, 0.05, 1.0},
 {1.0, 1.0, 0.7},
	{1.0,1.0,1.0},
	{1.0,1.0,1.0} };
};

bool command_point = 0;
GLchar *vertexsource, *fragmentsource; // �ҽ��ڵ� ���� ����
GLuint vertexshader, fragmentshader; // ���̴� ��ü
GLuint s_program;

random_device rd;
mt19937 rng(rd());
void TimerFunction(int value);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mousedrag(int, int);
void Mouse(int, int, int, int);
void Keyboard(unsigned char, int, int);
void spKeyboard(int, int, int);
void make_vertexShaders();
void make_fragmentShaders();
bool COMMAND_timer = false;
bool COMMAND_move = false;
bool COMMAND_linemode = false;
bool COMMAND_size = false;
bool tri_iso_switch = false;
bool COMMAND_click = false;
bool COMMAND_Rx = false;
bool COMMAND_Ry = false;
bool COMMAND_cull = true;	//�������� ����
bool COMMAND_dim = false;	//false�� 3����
bool COMMAND_SpaceR = false;
bool COMMAND_proj = true;
bool COMMAND_lightRotate = false;
bool COMMAND_SnowFalling = false;
bool COMMAND_lightPlusRotate = false;
bool COMMAND_lightMinusRotate = false;

int COMMAND_shape = 0;	//0�� ������ü, 1�� �ﰢ��
glm::vec4 myvec(1.0, 1.0, 1.0, 1.0);
glm::mat4 scaleMatrix = glm::mat4(1.0f);	//���� ��ķ� �ʱ�ȭ

GLfloat orthoSize = 5.0;		//��������

testmen test;
CubeManager cube;
CubeManager cm;
HalfDiamondManager hdm;
linemanager lm[3];
trimanager ltot;		//line ���� �ﰢ������	2��°���� ���������� ������
trimanager tm;
RectManager rm;
RectManager ttor;		//�ﰢ������ �簢��		2,3 ��°���� ���������� ������
PentaManager ptod;		//���������� ��(dot) ���� 5���� �۾���
PentaManager rtop;		//�簢������ ������   23��°
GLint tmOutcnt = 0;		//���� �ﰢ�� ��ִ���
GLint tmIncnt = 2;		//���� �ﰢ�� � �ִ���
GLint shapecnt = 0;   //���° �ﰢ���� �����ߴ���
GLfloat tri_iso = TRI_ISO;	//�ﰢ�� ���� �� default = tri_iso

unsigned int texture1, texture2, texture3, texture4, texture5, texture6;	//���� �ؽ����ҰŸ� �ʿ��Ѱ���

//ȸ����
GLfloat Rxr = 0;
GLfloat Ryr = 0;
GLfloat Rzr = 0;
//�̵���
GLfloat Txt = -0.0;
GLfloat Tyt = -0.0;
GLfloat Tzt = -0.0;
//ī�޶� ����
GLfloat CameraEyeTxt = 0.0f, CameraAtTxt = 0.0f;
GLfloat CameraEyeTyt = 1.0f, CameraAtTyt = 0.0f;
GLfloat CameraEyeTzt = 1.0f, CameraAtTzt = 0.0f;
GLfloat CameraTheta = 270;
//vbo�� [4][2] �� �ϰ� �ϸ� �翬�� �Ǵµ�
// vob[2] �� �ϰ� �׳� �Ҵ����൵ �ȴ� ? �����¹���
GLuint HDvao, HDvbo[2];
GLuint Cvao, Cvbo[2];
GLuint Tvao[2], Tvbo[2][2];
GLuint Lvao[3], Lvbo[3][2];
GLuint Rvao, Rvbo[2];
GLuint Pvao[2], Pvbo[2][2];
GLuint Tmvao, Tmvbo[2];
GLuint HDEBO;
GLuint CEBO;
GLuint EBO;
//��� ���� �͵�
GLfloat LightR = 1.0, LightG = 1.0, LightB = 1.0;
GLfloat LightX = 0.0, LightY = 0.8, LightZ = 1.0;
GLuint Lighttheta = 0;
//
//void initHDBuffer() //�ﰢ�Թ���
//{
//	glGenVertexArrays(1, &HDvao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
//	glBindVertexArray(HDvao); //--- VAO�� ���ε��ϱ�
//	glGenBuffers(2, HDvbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
//
//	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
//	glBindBuffer(GL_ARRAY_BUFFER, HDvbo[0]);
//	// ���� triShape ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
//	// triShape �迭�� ������: 9 * float
//	glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(GLfloat), hdm.HDShape, GL_STATIC_DRAW);
//
//	//---EBO���
//	glGenBuffers(1, &HDEBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, HDEBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(GLuint), hdm.index, GL_STATIC_DRAW);
//	//
//	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	// attribute �ε��� 0���� ��밡���ϰ� ��
//	glEnableVertexAttribArray(0);
//	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
//	glBindBuffer(GL_ARRAY_BUFFER, HDvbo[1]);
//	// ���� colors���� ���ؽ� ������ �����Ѵ�.
//	// colors �迭�� ������: 3 * TRI_POINT ��ŭ
//	glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(GLfloat), hdm.colors, GL_STATIC_DRAW);
//	// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	// attribute �ε��� 1���� ��� �����ϰ� ��.
//	glEnableVertexAttribArray(1);
//}
void InitLBuffer()
{
	for (int i = 0; i < 3; ++i)	//1�� ptod 
	{
		glGenVertexArrays(1, &Lvao[i]); //--- VAO �� �����ϰ� �Ҵ��ϱ�
		glBindVertexArray(Lvao[i]); //--- VAO�� ���ε��ϱ�
		glGenBuffers(2, Lvbo[i]); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

		//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
		glBindBuffer(GL_ARRAY_BUFFER, Lvbo[i][0]);
		// ���� triShape ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
		// triShape �迭�� ������: 9 * float
		glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), lm[i].LShape, GL_STATIC_DRAW);

		// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// attribute �ε��� 0���� ��밡���ϰ� ��
		glEnableVertexAttribArray(0);
		//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
		glBindBuffer(GL_ARRAY_BUFFER, Lvbo[i][1]);
		// ���� colors���� ���ؽ� ������ �����Ѵ�.
		// colors �迭�� ������: 3 * TRI_POINT ��ŭ
		glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), lm[i].colors, GL_STATIC_DRAW);
		// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// attribute �ε��� 1���� ��� �����ϰ� ��.
		glEnableVertexAttribArray(1);
	}

}

void InitPBuffer()
{
	for (int i = 0; i < 2; ++i)	//1�� ptod 
	{
		glGenVertexArrays(1, &Pvao[i]); //--- VAO �� �����ϰ� �Ҵ��ϱ�
		glBindVertexArray(Pvao[i]); //--- VAO�� ���ε��ϱ�
		glGenBuffers(2, Pvbo[i]); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

		//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
		glBindBuffer(GL_ARRAY_BUFFER, Pvbo[i][0]);
		// ���� triShape ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
		// triShape �迭�� ������: 9 * float
		if (i == 0)
			glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(GLfloat), ptod.PShape, GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(GLfloat), rtop.PShape, GL_STATIC_DRAW);

		// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// attribute �ε��� 0���� ��밡���ϰ� ��
		glEnableVertexAttribArray(0);
		//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
		glBindBuffer(GL_ARRAY_BUFFER, Pvbo[i][1]);
		// ���� colors���� ���ؽ� ������ �����Ѵ�.
		// colors �迭�� ������: 3 * TRI_POINT ��ŭ
		if (i == 0)
			glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(GLfloat), ptod.colors, GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(GLfloat), rtop.colors, GL_STATIC_DRAW);
		// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// attribute �ε��� 1���� ��� �����ϰ� ��.
		glEnableVertexAttribArray(1);
	}
}


void InitRBuffer()
{
	glGenVertexArrays(1, &Rvao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(Rvao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, Rvbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, Rvbo[0]);
	// ���� triShape ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), rm.RShape, GL_STATIC_DRAW);

	//---EBO���
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), rm.index, GL_STATIC_DRAW);
	//
	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, Rvbo[1]);
	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: 3 * TRI_POINT ��ŭ
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), rm.colors, GL_STATIC_DRAW);
	// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}


void InitTBuffer()
{

}

void initLine()
{
	lm[1].LShape[0][0] = 1.0;
	lm[1].LShape[0][1] = 0.0;
	lm[1].LShape[0][2] = 0.0;
	lm[1].LShape[1][0] = -1.0;
	lm[1].LShape[1][1] = 0.0;
	lm[1].LShape[1][2] = 0.0;

	lm[1].colors[0][0] = 0.0;
	lm[1].colors[1][0] = 0.0;

	lm[1].colors[0][1] = 1.0;
	lm[1].colors[1][1] = 1.0;

	lm[2].LShape[0][0] = 0.0;
	lm[2].LShape[0][1] = 0.0;
	lm[2].LShape[0][2] = 1.0;
	lm[2].LShape[1][0] = 0.0;
	lm[2].LShape[1][1] = 0.0;
	lm[2].LShape[1][2] = -1.0;

	lm[2].colors[0][0] = 0.0;
	lm[2].colors[1][0] = 0.0;

	lm[2].colors[0][2] = 1.0;
	lm[2].colors[1][2] = 1.0;
}
void initPenta()
{
	rtop.PShape[0][0] = -0.5;
	rtop.PShape[0][1] = -0.5;
	rtop.PShape[0][2] = 0;

	rtop.PShape[1][0] = -0.625;
	rtop.PShape[1][1] = -0.5;
	rtop.PShape[1][2] = 0;

	rtop.PShape[2][0] = -0.75;
	rtop.PShape[2][1] = -0.5;
	rtop.PShape[2][2] = 0;

	rtop.PShape[3][0] = -0.75;
	rtop.PShape[3][1] = -0.75;
	rtop.PShape[3][2] = 0;

	rtop.PShape[4][0] = -0.5;
	rtop.PShape[4][1] = -0.75;
	rtop.PShape[4][2] = 0;





}

void InitShader()
{
	make_vertexShaders();
	make_fragmentShaders();
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(s_program, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�

	if (!result) {
		glGetProgramInfoLog(s_program, 512, NULL, errorLog);
		cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
		return;
	}
	//checkCompileErrors(s_program, "PROGRAM");
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}



void ReadObj(const char* file, vector<GLfloat>& outvertex, vector<GLfloat>& outnormal, vector<GLfloat>& outtexture)
{

	FILE *fptr = NULL;
	fopen_s(&fptr, file, "rb"); /* Open file for reading */

	if (fptr == NULL) /* Return NULL on failure */
	{
		cout << "������ ã���� ����" << endl;
		return;
	}

	//--- 1. ��ü ���ؽ� ���� �� �ﰢ�� ���� ����
	char count[100];
	char bind[100];
	int vertexNum = 0;
	int normalNum = 0;
	int textureNum = 0;
	int faceNum = 0;
	while (!feof(fptr)) {
		fscanf(fptr, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'vn' && count[1] == '\0')
			normalNum += 1;
		else if (count[0] == 'vt' && count[1] == '\0')
			textureNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count)); // �迭 �ʱ�ȭ
	}
	//--- 2. �޸� �Ҵ�
	int vertIndex = 0;
	int norIndex = 0;
	int texIndex = 0;
	int faceIndex = 0;
	fclose(fptr);
	fopen_s(&fptr, file, "rb"); /* Open file for reading */

	if (fptr == NULL) /* Return NULL on failure */
	{
		cout << "������ ã���� ����" << endl;
		return;
	}
	vector<glm::vec3> v;
	vector<glm::vec3> vn;
	vector<glm::vec3> vt;
	vector<int> vface;
	vector<int> nface;
	vector<int> tface;
	glm::vec3 tmp;
	//--- 3. �Ҵ�� �޸𸮿� �� ���ؽ�, ���̽� ���� �Է�
	while (!feof(fptr)) {
		fscanf(fptr, "%s", bind);
		if (bind[0] == 'v' && bind[1] == 'n' && bind[2] == '\0') {
			fscanf(fptr, "%f %f %f",
				&tmp.x, &tmp.y,
				&tmp.z);
			vn.push_back(tmp);
			norIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
			fscanf(fptr, "%f %f %f",
				&tmp.x, &tmp.y,
				&tmp.z);
			vt.push_back(tmp);
			texIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(fptr, "%f %f %f",
				&tmp.x, &tmp.y,
				&tmp.z);
			v.push_back(tmp);
			vertIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			fscanf(fptr, "%f/%f/%f", &tmp.x, &tmp.y, &tmp.z);
			vface.push_back(tmp.x), tface.push_back(tmp.y), nface.push_back(tmp.z);	//x��ǥ
			fscanf(fptr, "%f/%f/%f", &tmp.x, &tmp.y, &tmp.z);
			vface.push_back(tmp.x), tface.push_back(tmp.y), nface.push_back(tmp.z);	//y��ǥ
			fscanf(fptr, "%f/%f/%f", &tmp.x, &tmp.y, &tmp.z);
			vface.push_back(tmp.x), tface.push_back(tmp.y), nface.push_back(tmp.z);	//z��ǥ
			faceIndex++;
		}
	}
	fclose(fptr);

	outvertex.resize(vface.size() * 3, GLfloat());
	outnormal.resize(vface.size() * 3, GLfloat());
	outtexture.resize(vface.size() * 2, GLfloat());

	for (auto i = 0, j = 0, k = 0; i < vface.size() * 3; i += 3, j++, k += 2)
	{
		outvertex[i] = v[vface[j] - 1].x;
		outvertex[i + 1] = v[vface[j] - 1].y;
		outvertex[i + 2] = v[vface[j] - 1].z;
		outtexture[k] = vt[tface[j] - 1].x;
		outtexture[k + 1] = vt[tface[j] - 1].y;
		outnormal[i] = vn[nface[j] - 1].x;
		outnormal[i + 1] = vn[nface[j] - 1].y;
		outnormal[i + 2] = vn[nface[j] - 1].z;

	}



}


char* filetobuf(const char *file)
{
	FILE *fptr;
	long length;
	char *buf;

	fopen_s(&fptr, file, "rb"); /* Open file for reading */

	if (fptr == NULL) /* Return NULL on failure */
	{
		cout << "������ ã���� ����" << endl;
		return NULL;
	}

	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */

	return buf; /* Return the buffer */
}


void InitTexture()
{
	BITMAPINFO *bmp;
	int img_w = 0, img_h = 0, numOfchannel = 0;
	int format = GL_RGB;
	//--texture 1
	glGenTextures(1, &texture1); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture1); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("123.png", &img_w, &img_h, &numOfchannel, 0);
	if (numOfchannel == 4)
	{
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, img_w, img_h, 0, format, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
	//--texture 2
	glGenTextures(1, &texture2); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture2); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("pic1.png", &img_w, &img_h, &numOfchannel, 0);
	if (numOfchannel == 4)
	{
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, img_w, img_h, 0, format, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
	//--texture 3
	glGenTextures(1, &texture3); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture3); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("pic2.png", &img_w, &img_h, &numOfchannel, 0);
	if (numOfchannel == 4)
	{
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, img_w, img_h, 0, format, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
	//--texture 4
	glGenTextures(1, &texture4); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture4); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("pic3.png", &img_w, &img_h, &numOfchannel, 0);
	if (numOfchannel == 4)
	{
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, img_w, img_h, 0, format, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
	//--texture 5
	glGenTextures(1, &texture5); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture5); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("pic4.png", &img_w, &img_h, &numOfchannel, 0);
	if (numOfchannel == 4)
	{
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, img_w, img_h, 0, format, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����
	//--texture 6
	glGenTextures(1, &texture6); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture6); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("pic5.png", &img_w, &img_h, &numOfchannel, 0);
	if (numOfchannel == 4)
	{
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, img_w, img_h, 0, format, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����















	glUseProgram(s_program);
	int tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, 0); //--- ���÷��� 0�� �������� ����

	stbi_image_free(data);
}


void make_vertexShaders()
{


	vertexsource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result = 0;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		return;
	}
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{


	GLint width = WINDOW_WIDTH;
	GLint height = WINDOW_HEIGHT;
	GLfloat rColor, gColor, bColor;
	rColor = gColor = bColor = 1.0;
	//--- ������ �����ϱ�
	glutInit(&argc, argv);// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(width, height); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ���� (������ �̸�)
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����
	InitShader();
	initPenta();
	initLine();
	InitLBuffer();
	InitTBuffer();
	InitRBuffer();
	InitPBuffer();
	//initHDBuffer();
	cube.initCubeBuffer();
	InitTexture();
	test.initBuffer();

	ReadObj("pyramid.obj", hdm.vertex, hdm.normal, hdm.texture);
	hdm.initHalfDiamondBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(spKeyboard);
	glutMotionFunc(Mousedrag);
	glutTimerFunc(1, TimerFunction, 1);
	glutMainLoop();
}


void TimerFunction(int value)
{
	if (COMMAND_lightPlusRotate || COMMAND_lightMinusRotate)
	{

		LightX = 0.5 * cosf(Lighttheta *3.141592 / 180);
		LightZ = 0.5 * sinf(Lighttheta*3.141592 / 180);
		if (COMMAND_lightPlusRotate)
			Lighttheta = Lighttheta + 1 % 360;
		else
		{
			Lighttheta -= 1;
			if (Lighttheta < 0)
				Lighttheta = 360;

		}

	}

	if (COMMAND_Rx == true)
		Rxr = Rxr + 1 % 360;
	if (COMMAND_Ry == true)
		Ryr = Ryr + 1 % 360;

	//Vao�� �ִ� vbo �缳�����ֱ�
	glutPostRedisplay();

	glutTimerFunc(10, TimerFunction, 1);
}

void spKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		Txt -= 0.01;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		Txt += 0.01;
	}
	else if (key == GLUT_KEY_UP)
	{
		Tyt += 0.01;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		Tyt -= 0.01;
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	uniform_real_distribution<GLfloat> XYR(-0.5000, 0.5000);
	uniform_real_distribution<GLfloat> XYP1(0.5000, 1.0000);
	uniform_real_distribution<GLfloat> XYP2(-1.0000, -0.5000);



	uniform_int_distribution<int> selectR(0, 1);
	uniform_real_distribution<GLfloat> RGBR(0.00, 1.00);

	if (key == 'm')
	{
		COMMAND_proj = (COMMAND_proj + 1) % 2;
	}
	else if (key == 'r' || key == 'R')
	{
		if (key == 'r')
		{
			COMMAND_lightPlusRotate = true;
			COMMAND_lightMinusRotate = false;
		}
		else {

			COMMAND_lightPlusRotate = false;
			COMMAND_lightMinusRotate = true;
		}
	}
	else if (key == 'c')
	{
		COMMAND_shape = 0;
	}
	else if (key == 'p')
	{
		COMMAND_shape = 1;
	}
	else if (key == 's')
	{
		Rxr = 0;
		Ryr = 0;
		Txt = 0;
		Tyt = 0;
		COMMAND_Rx = false;
		COMMAND_Ry = false;
	}
	else if (key == 'h')
	{
		if (COMMAND_cull)
		{
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			COMMAND_cull = false;
		}
		else {
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			COMMAND_cull = true;
		}
	}
	else if (key == 'q')
	{
		exit(0);
	}
	else if (key == 't')
	{
		COMMAND_timer = (COMMAND_timer + 1) % 2;
		//glutTimerFunc(1, TimerFunction, 1);

	}
	else if (key == 'w' || key == 'W')
	{
		COMMAND_linemode = (COMMAND_linemode + 1) % 2;

		if (COMMAND_linemode == true)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (key == 'x' || key == 'X')
	{
		COMMAND_Rx = (COMMAND_Rx + 1) % 2;

	}
	else if (key == 'y' || key == 'Y')
	{
		COMMAND_Ry = (COMMAND_Ry + 1) % 2;
	}

	glutPostRedisplay();
}

void Mousedrag(int x, int y)
{
	if (COMMAND_click == false)
		return;
	GLfloat ox, oy;

	//�Ѻ��� 0.1�� ���ﰢ��
	ox = (float)(x - (float)WINDOW_WIDTH / 2.0)*(float)(1.0 / (float)(WINDOW_WIDTH / 2.0));
	oy = -(y - WINDOW_HEIGHT / 2.0)*(1.0 / (WINDOW_HEIGHT / 2.0));
	//UpdateBuffer();


	glutPostRedisplay();
}
void Mouse(int button, int state, int x, int y)
{
	uniform_real_distribution<float> RGBR(0.00, 1.00);
	uniform_real_distribution<GLfloat> RR(0.003, 0.005);

	uniform_int_distribution<int> SR(0, 1);
	random_device rd;
	mt19937 rng(rd());
	GLfloat ox, oy;
	GLfloat triside = 0.5;
	//�Ѻ��� 0.1�� ���ﰢ��
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		ox = (float)(x - (float)WINDOW_WIDTH / 2.0)*(float)(1.0 / (float)(WINDOW_WIDTH / 2.0));
		oy = -(y - WINDOW_HEIGHT / 2.0)*(1.0 / (WINDOW_HEIGHT / 2.0));

	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		COMMAND_click = false;
	}
	glutPostRedisplay();
}
GLUquadricObj *qobj;

void clearTRbuf(glm::mat4& Tx, glm::mat4& Ty, glm::mat4& Tz, glm::mat4& Rx, glm::mat4& Ry, glm::mat4& Rz, glm::mat4& S)
{
	Tx = glm::mat4(1.0f);
	Ty = glm::mat4(1.0f);
	Tz = glm::mat4(1.0f);
	Ry = glm::mat4(1.0f);
	Rx = glm::mat4(1.0f);
	Rz = glm::mat4(1.0f);
	S = glm::mat4(1.0f);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
		//glClearColor(rColor, gColor, bColor, 1.0f);
	glClearColor(1.0, 1.0, 1.0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	// ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);
	glm::mat4 Tx = glm::mat4(1.0f); //--- translation matrix
	glm::mat4 Ty = glm::mat4(1.0f); //--- translation matrix
	glm::mat4 Tz = glm::mat4(1.0f); //--- translation matrix
	glm::mat4 Ry = glm::mat4(1.0f); //--- rotation matrix
	glm::mat4 Rx = glm::mat4(1.0f); //--- rotation matrix
	glm::mat4 Rz = glm::mat4(1.0f); //--- rotation matrix
	glm::mat4 S = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f); //--- transformation matrix

	glm::mat4 vTransform = glm::mat4(1.0f);	//�亯ȯ
	glm::mat4 pTransform = glm::mat4(1.0f);	//������ȯ


	unsigned int modelLoc = glGetUniformLocation(s_program, "model");
	unsigned int viewLoc = glGetUniformLocation(s_program, "view");
	unsigned int projLoc = glGetUniformLocation(s_program, "projection");
	unsigned int colorLoc = glGetUniformLocation(s_program, "Colors");
	unsigned int viewPosLoc = glGetUniformLocation(s_program, "viewPos");
	unsigned int lightPosLoc = glGetUniformLocation(s_program, "lightPos");
	unsigned int lightColorLoc = glGetUniformLocation(s_program, "lightColor");



	//�亯ȯ
	glm::vec3 cameraEye = glm::vec3(CameraEyeTxt, CameraEyeTyt, CameraEyeTzt);
	glm::vec3 cameraAt = glm::vec3(CameraAtTxt, CameraAtTyt, CameraAtTzt);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	//POS = EYE , Direction = AT
	vTransform = glm::lookAt(cameraEye, cameraAt, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(vTransform));
	//������ȯ

	if (COMMAND_proj == false)
		pTransform = glm::perspective(glm::radians(45.0f), (float)(WINDOW_WIDTH / WINDOW_HEIGHT), 0.01f, 10.0f);
	else if (COMMAND_proj == true)
		pTransform = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, -orthoSize, orthoSize);

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pTransform));

	//����ֱ����� �ɸ��� ����
	glUniform3f(colorLoc, 1.0, 1.0, 1.0);
	glUniform3f(viewPosLoc, cameraEye.x, cameraEye.y, cameraEye.z);
	glUniform3f(lightPosLoc, LightX, LightY, LightZ);
	glUniform3f(lightColorLoc, LightR, LightG, LightB);



	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TR));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(vTransform));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pTransform));



	clearTRbuf(Tx, Ty, Tz, Rx, Ry, Rz, S);
	Tx = glm::translate(Tx, glm::vec3(Txt, 0.0, 0.0)); //--- x������ translation
	Ty = glm::translate(Ty, glm::vec3(0.0, Tyt, 0.0)); //--- y������ translation
	Tz = glm::translate(Tz, glm::vec3(0.0, 0.0, Tzt)); //--- y������ translation
	Rx = glm::rotate(Ry, glm::radians(Rxr), glm::vec3(1.0, 0.0, 0.0)); //--- x�࿡���Ͽ� ȸ��
	Ry = glm::rotate(Ry, glm::radians(Ryr), glm::vec3(0.0, 1.0, 0.0)); //--- y�࿡���Ͽ� ȸ��
	Rz = glm::rotate(Ry, glm::radians(Rzr), glm::vec3(0.0, 0.0, 1.0)); //--- z�࿡���Ͽ� ȸ��
	S = glm::scale(S, glm::vec3(0.1, 0.1, 0.1));
	TR = TR * Tx * Ty * Tz * Rx * Ry *Rz*S;
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TR));



	//�ѤѤѤ� ���
	glUseProgram(s_program);
	if (COMMAND_shape == 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			switch (i)
			{
			case 0:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture1);
				break;
			case 1:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture2);
				break;
			case 2:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture3);
				break;
			case 3:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture4);
				break;
			case 4:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture5);
				break;
			case 5:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture6);
				break;
			}
			glBindVertexArray(cube.vao[i]);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(cube.normal) / sizeof(GLfloat));
		}
	}
	else if (COMMAND_shape == 1)
	{
		for (int i = 0; i < 8; ++i)
		{
			switch (i)
			{
			case 0:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture1);
				break;
			case 1:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture2);
				break;
			case 2:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture3);
				break;
			case 3:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture4);
				break;
			case 4:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture5);
				break;
			case 5:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture6);
				break;
			case 6:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture4);
				break;
			case 7:
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture4);
				break;
			}
			glBindVertexArray(hdm.vao[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		/*	glBindVertexArray(HDvao);
			glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);*/

	}

	//���

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�





}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}