#include <iostream>
#include <random>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

//윈도우 크기
#define WIDTH 500
#define HEIGHT 500

//갯수
#define COUNT 1 //0 바닥 1 몸체아래 2몸체위 3원기둥왼쪽 4 원기둥 오른쪽
#define RC 7 //로봇 몸조각수

//CUBE == STAGE
#define CP 8    //CUBE POINT 
#define CI 36	//CUBE INDEX
#define CFI 6   //CUBE FRONT INDEX
#define SF 4	//STAGE FRONT POINT

//도형의 크기
#define SGSZ 1.0 //STAGE SIZE
#define BTSZ 1.0
#define RBSZ 1 //ROBOT SIZE
#define CMY 0.0 //이건뭐지
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 projTZ = glm::mat4(1.0f);
glm::mat4 projTX = glm::mat4(1.0f);
glm::mat4 projRY = glm::mat4(1.0f);



glm::vec3 cameraPos2 = glm::vec3(0.0f, 1.0f, 8.0f);


float NCL(int c);
float randomRGB();
GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid ArrowKey(int, int, int);

GLint select = 0, t = 100 , df = 0, temp = -1;
GLboolean w = false, h = true, stop = true, open = false, rt = false, jm = false;

GLchar *vertexsource, *fragmentsource;
GLuint vertexshader, fragmentshader;
GLuint vao[COUNT], vbo[COUNT][2], ebo[COUNT];
GLuint s_program;
unsigned int modelLocation, projectionLocation, viewLocation,vColorLocation, flagLocation;



struct Stage {
	
	GLuint vao[2],vbo[2][1],ebo[2];

	GLfloat cube_pos[CP][3] = {
	-SGSZ,-SGSZ,SGSZ, //왼쪽앞아래
	SGSZ,-SGSZ,SGSZ, // 오른쪽앞아래
	SGSZ,-SGSZ,-SGSZ, //오른쪽뒤아래
	-SGSZ,-SGSZ,-SGSZ, //왼쪽뒤아래
	-SGSZ,SGSZ,SGSZ,
	SGSZ,SGSZ,SGSZ,
	SGSZ,SGSZ,-SGSZ,
	-SGSZ,SGSZ,-SGSZ
	};

	GLint cube_index[CI] = {
	1,6,2,
	1,5,6,
	2,6,3,
	3,6,7,
	0,3,7,
	0,7,4,
	0,1,2,
    0,2,3
	};

	GLfloat cff_pos[SF][3] = {
	-SGSZ,SGSZ,0.0f, //0
	-SGSZ,-SGSZ,0.0f,  //1
	SGSZ,-SGSZ,0.0f,   //2
	SGSZ,SGSZ,0.0f  //3
	};

	GLint cff_index[6] = {
		0,1,2,
		0,2,3
	};

	GLfloat cube_color[CP][3];
	GLfloat cff_color[SF][3] = {
	0.5f,0.5f,0.5f,
	0.5f,0.5f,0.5f,
	0.5f,0.5f,0.5f,
	0.5f,0.5f,0.5f,
	};

	glm::mat4 FF = glm::mat4(1.0f);
	glm::mat4 FFTZ = glm::mat4(1.0f);
	glm::mat4 FFRX = glm::mat4(1.0f);
	
	GLfloat angle = 0;
	Stage()
	{
		//컬러 초기화
		for (int i = 0; i < CP; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cube_color[i][j] = randomRGB();
			}
		}
		FFTZ = glm::translate(FFTZ, glm::vec3(0.0f, 0.0f, SGSZ));
		
	}
	void InitBuffer()
	{
		for (int i = 0; i < 2; i++)
		{
			glGenVertexArrays(1, &vao[i]);
			glBindVertexArray(vao[i]);
			glGenBuffers(2, vbo[i]);

			//위치
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);
			if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(cube_pos), cube_pos, GL_STATIC_DRAW);
			if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(cff_pos), cff_pos, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			//색상
			//glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);
			//if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);
			//if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(cff_color), cff_color, GL_STATIC_DRAW);
			/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);*/

			//EBO 설정

			glGenBuffers(1, &ebo[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
			if (i == 0) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index), cube_index, GL_STATIC_DRAW);
			if (i == 1) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cff_index), cff_index, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
		}
		
	}
	void Update()
	{
		FFRX = glm::mat4(1.0f);

		if(angle > -90.0f && df == -1) angle += df * 10.0f;
		else if (angle < 0.0f && df == 1) angle += df * 10.0f;

		FFRX = glm::rotate(FFRX, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

		
	}

	void Draw()
	{
		FF = FFRX * FFTZ;

		
		for (int i = 0; i < 2; i++)
		{
			glBindVertexArray(vao[i]);
			if (i == 0) {
				glUniform3f(vColorLocation, 0.3f, 0.3f, 0.3f);
				glDrawElements(GL_TRIANGLES, CI, GL_UNSIGNED_INT, 0);
			}
			if (i == 1)
			{

				glUniform3f(vColorLocation, 0.1f, 0.1f, 0.1f);
				
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(FF));
				glDrawElements(GL_TRIANGLES, CFI, GL_UNSIGNED_INT, 0);
			}
		}
		
	}

};

struct Camera {
	glm::vec3 cameraPos;
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 T = glm::mat4(1.0f);
	glm::mat4 Tz = glm::mat4(1.0f);
	glm::mat4 Ry = glm::mat4(1.0f);
	glm::mat4 Ry2 = glm::mat4(1.0f);

	int z = 0, y = 0;

	Camera()
	{
		cameraPos = glm::vec3(0.0f, CMY, 5.0f);
	}

	void init()
	{
		cameraPos = glm::vec3(0.0f, CMY, 5.0f);
		T = glm::mat4(1.0f);
		Ry = glm::mat4(1.0f);
		Ry2 = glm::mat4(1.0f);
	}

	void CMTranslate() // z/Z 앞/뒤
	{
		cameraPos = glm::vec3(0.0f, CMY, 5.0f + z * -0.1f);
	}
	void CMRotate(int i) // y/Y 자전
	{
		Ry = glm::rotate(Ry, glm::radians(i*10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
	}
	void CMRotate2(int i) // r/R 공전
	{
	
		Ry2 = glm::rotate(Ry2, glm::radians(i*10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
	}
	void cameraset()
	{
		
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		view = Ry * view * Ry2;
		viewLocation = glGetUniformLocation(s_program, "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	}
};

struct Robot {
	float HEAD = 0.02*RBSZ ,BODY = 0.04 * RBSZ,ARM = 0.007 * RBSZ, LEG = 0.01 * RBSZ, NOSE = 0.005 * RBSZ;

	bool turn = false;
	int dir = 1,dx=0,dz = 1,dlgr = 1, damr = 1, dy = 1 ; 
	float pos[3] = {0.0f,}, angle = 180;

	float lgr = 0.0f, ddy = 0.05f;

	glm::mat4 W = glm :: mat4(1.0f);
	glm::mat4 T = glm::mat4(1.0f);
	glm::mat4 J = glm::mat4(1.0f);
	glm::mat4 R = glm::mat4(1.0f);
	glm::mat4 RB = glm::mat4(1.0f);

	glm::mat4 LG = glm::mat4(1.0f);
	glm::mat4 LGIT = glm::mat4(1.0f);
	glm::mat4 LG2 = glm::mat4(1.0f);
	glm::mat4 LGIT2 = glm::mat4(1.0f);

	glm::mat4 ALRX = glm::mat4(1.0f);
	glm::mat4 ALRX2 = glm::mat4(1.0f);
	glm::mat4 ALTY = glm::mat4(1.0f);
	glm::mat4 ALGTY2 = glm::mat4(1.0f);
	
	glm::mat4 BD = glm::mat4(1.0f);
	glm::mat4 BDIT = glm::mat4(1.0f);

	glm::mat4 HD = glm::mat4(1.0f);
	glm::mat4 HDIT = glm::mat4(1.0f);

	glm::mat4 AM = glm::mat4(1.0f);
	glm::mat4 AMIT = glm::mat4(1.0f);
	glm::mat4 AM2 = glm::mat4(1.0f);
	glm::mat4 AMIT2 = glm::mat4(1.0f);
	glm::mat4 AMTZY = glm::mat4(1.0f);
	glm::mat4 AMTZY2 = glm::mat4(1.0f);
	glm::mat4 AMRX = glm::mat4(1.0f);
	glm::mat4 AMRX2 = glm::mat4(1.0f);

	glm::mat4 NS = glm::mat4(1.0f);
	glm::mat4 NSIT = glm::mat4(1.0f);

	

	GLuint vao[RC], vbo[RC][2], ebo[RC];

	GLfloat head_pos[CP][3] = {
	-HEAD,-HEAD,HEAD,
	HEAD,-HEAD,HEAD,
	HEAD,-HEAD,-HEAD,
	-HEAD,-HEAD,-HEAD,
	-HEAD,HEAD,HEAD,
	HEAD,HEAD,HEAD,
	HEAD,HEAD,-HEAD,
	-HEAD,HEAD,-HEAD
	};
	GLfloat leg_pos[CP][3] = {
	-LEG,-LEG*4,LEG,
	LEG,-LEG*4,LEG,
	LEG,-LEG*4,-LEG,
	-LEG,-LEG*4,-LEG,
	-LEG,LEG*0,LEG,
	LEG,LEG*0,LEG,
	LEG,LEG*0,-LEG,
	-LEG,LEG*0,-LEG
	};
	GLfloat body_pos[CP][3] = {
	-BODY,-BODY,BODY,
	BODY,-BODY,BODY,
	BODY,-BODY,-BODY,
	-BODY,-BODY,-BODY,
	-BODY,BODY  ,BODY,
	BODY,BODY  ,BODY,
	BODY,BODY  ,-BODY,
	-BODY,BODY  ,-BODY
	};
	GLfloat arm_pos[CP][3] = {
	-ARM,-ARM*7,ARM,
	ARM,-ARM*7,ARM,
	ARM,-ARM*7,-ARM,
	-ARM,-ARM*7,-ARM,
	-ARM,ARM*0  ,ARM,
	ARM,ARM*0  ,ARM,
	ARM,ARM*0  ,-ARM,
	-ARM,ARM*0  ,-ARM
	};
	GLfloat nose_pos[CP][3] = {
	-NOSE,-NOSE ,NOSE,
	NOSE,-NOSE,NOSE,
	NOSE,-NOSE,-NOSE,
	-NOSE,-NOSE,-NOSE,
	-NOSE,NOSE ,NOSE,
	NOSE,NOSE,NOSE,
	NOSE,NOSE,-NOSE,
	-NOSE,NOSE,-NOSE
	};

	GLint cube_index[CI] = {
	0,1,4,
	1,5,4,
	4,5,6,
	4,6,7,
	1,2,6,
	1,6,5,
	2,3,6,
	3,7,6,
	0,7,3,
	0,4,7,
	0,2,1,
	0,3,2,
	};

	GLfloat leg_color[CP][3]
	{
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
	};
	GLfloat leg_color2[CP][3]
	{
		0.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
	};
	GLfloat body_color[CP][3]
	{
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
	};
	GLfloat head_color[CP][3]
	{
		0.54f,0.0f,1.0f,
		0.54f,0.0f,1.0f,
		0.54f,0.0f,1.0f,
		0.54f,0.0f,1.0f,
		0.54f,0.0f,1.0f,
		0.54f,0.0f,1.0f,
		0.54f,0.0f,1.0f,
	};
	GLfloat arm_color[CP][3]
	{
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
	};
	GLfloat arm_color2[CP][3]
	{
		1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
	};
	GLfloat nose_color[CP][3]
	{
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
	};

	Robot()
	{
		LGIT = glm::translate(LGIT, glm::vec3(-0.02f*RBSZ, -SGSZ+LEG*4, 0.0f));
		LGIT2 = glm::translate(LGIT2, glm::vec3(0.02f*RBSZ, -SGSZ + LEG * 4 +0.01, 0.0f));

		AMIT = glm::translate(AMIT, glm::vec3(-BODY-ARM*1.5, -SGSZ + LEG * 8+BODY , 0.0f));
		AMIT2 = glm::translate(AMIT2, glm::vec3(BODY+ARM*1.5, -SGSZ + LEG * 8 + BODY, 0.0f));

		BDIT = glm::translate(BDIT, glm::vec3(0.0f, -SGSZ+LEG*8, 0.0f));

		HDIT = glm::translate(HDIT, glm::vec3(0.0f, -SGSZ + LEG * 8 + BODY*1.5, 0.0f));

		NSIT = glm::translate(NSIT, glm::vec3(0.0f, -SGSZ + LEG * 8 + BODY * 1.5 , HEAD));

		
		
	}

	void init()
	{
		 W = glm::mat4(1.0f);
		 T = glm::mat4(1.0f);
		 J = glm::mat4(1.0f);
		 R = glm::mat4(1.0f);
		 RB = glm::mat4(1.0f);

		 LG = glm::mat4(1.0f);
		 LGIT = glm::mat4(1.0f);
		 LG2 = glm::mat4(1.0f);
		 LGIT2 = glm::mat4(1.0f);

		 ALRX = glm::mat4(1.0f);
		 ALRX2 = glm::mat4(1.0f);
		 ALTY = glm::mat4(1.0f);
		  ALGTY2 = glm::mat4(1.0f);

		  BD = glm::mat4(1.0f);
		  BDIT = glm::mat4(1.0f);

		  HD = glm::mat4(1.0f);
		  HDIT = glm::mat4(1.0f);

		  AM = glm::mat4(1.0f);
		  AMIT = glm::mat4(1.0f);
		  AM2 = glm::mat4(1.0f);
		  AMIT2 = glm::mat4(1.0f);
		  AMTZY = glm::mat4(1.0f);
		  AMTZY2 = glm::mat4(1.0f);
		  AMRX = glm::mat4(1.0f);
		  AMRX2 = glm::mat4(1.0f);

		  NS = glm::mat4(1.0f);
		  NSIT = glm::mat4(1.0f);

		LGIT = glm::translate(LGIT, glm::vec3(-0.02f*RBSZ, -SGSZ + LEG * 4, 0.0f));
		LGIT2 = glm::translate(LGIT2, glm::vec3(0.02f*RBSZ, -SGSZ + LEG * 4 + 0.01, 0.0f));

		AMIT = glm::translate(AMIT, glm::vec3(-BODY - ARM * 1.5, -SGSZ + LEG * 8 + BODY, 0.0f));
		AMIT2 = glm::translate(AMIT2, glm::vec3(BODY + ARM * 1.5, -SGSZ + LEG * 8 + BODY, 0.0f));

		BDIT = glm::translate(BDIT, glm::vec3(0.0f, -SGSZ + LEG * 8, 0.0f));

		HDIT = glm::translate(HDIT, glm::vec3(0.0f, -SGSZ + LEG * 8 + BODY * 1.5, 0.0f));

		NSIT = glm::translate(NSIT, glm::vec3(0.0f, -SGSZ + LEG * 8 + BODY * 1.5, HEAD));
		 turn = false;
		dir = 1, dx = 0, dz = 1, dlgr = 1, damr = 1, dy = 1;
		for(int i = 0; i<3; i++) pos[i] = { 0.0f, };
		angle = 180;

		lgr = 0.0f, ddy = 0.05f;
		
	}

	void swing()
	{
		ALRX = glm::mat4(1.0f);
		ALRX2 = glm::mat4(1.0f);

		
		lgr += dlgr * 10.0f;
		ALRX = glm::rotate(ALRX, glm::radians(lgr), glm::vec3(1.0f, 0.0f, 0.0f));
		ALRX2 = glm::rotate(ALRX2, glm::radians(-1*lgr), glm::vec3(1.0f, 0.0f, 0.0f));

		

		if (lgr >= 30.0f || lgr <= -30.0f)
		{
			dlgr *= -1;
			
		}
		
	}
	void InitBuffer()
	{
		for (int i = 0; i < RC; i++)
		{
			glGenVertexArrays(1, &vao[i]);
			glBindVertexArray(vao[i]);
			glGenBuffers(2, vbo[i]);

			//위치
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][0]);
			if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(leg_pos), leg_pos, GL_STATIC_DRAW);
			if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(leg_pos), leg_pos, GL_STATIC_DRAW);
			if (i == 2) glBufferData(GL_ARRAY_BUFFER, sizeof(body_pos), body_pos, GL_STATIC_DRAW);
			if (i == 3) glBufferData(GL_ARRAY_BUFFER, sizeof(head_pos), head_pos, GL_STATIC_DRAW);
			if (i == 4) glBufferData(GL_ARRAY_BUFFER, sizeof(arm_pos), arm_pos, GL_STATIC_DRAW);
			if (i == 5) glBufferData(GL_ARRAY_BUFFER, sizeof(arm_pos), arm_pos, GL_STATIC_DRAW);
			if (i == 6) glBufferData(GL_ARRAY_BUFFER, sizeof(nose_pos), nose_pos, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			//색상
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i][1]);
			if (i == 0) glBufferData(GL_ARRAY_BUFFER, sizeof(leg_color), leg_color, GL_STATIC_DRAW);
			if (i == 1) glBufferData(GL_ARRAY_BUFFER, sizeof(leg_color2), leg_color2, GL_STATIC_DRAW);
			if (i == 2) glBufferData(GL_ARRAY_BUFFER, sizeof(body_color), body_color, GL_STATIC_DRAW);
			if (i == 3) glBufferData(GL_ARRAY_BUFFER, sizeof(head_color), head_color, GL_STATIC_DRAW);
			if (i == 4) glBufferData(GL_ARRAY_BUFFER, sizeof(arm_color), arm_color, GL_STATIC_DRAW);
			if (i == 5) glBufferData(GL_ARRAY_BUFFER, sizeof(arm_color2), arm_color2, GL_STATIC_DRAW);
			if (i == 6) glBufferData(GL_ARRAY_BUFFER, sizeof(nose_color), nose_color, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			//EBO 설정

			glGenBuffers(1, &ebo[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
			if (i >= 0) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index), cube_index, GL_STATIC_DRAW);
			//if (i == 1) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cff_index), cff_index, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
		}

	}
	void TurnBack()
	{
		rt = true;
		turn = false;
		switch (dir)
		{
		case 0:
			dir = 1;

			break;
		case 1:
			dir = 0;

			break;
		case 2:
			dir = 3;
			
			break;
		case 3:
			dir = 2;
			
			break;

		}
	}
	void Jump()
	{
		J = glm::mat4(1.0f);
		J = glm::translate(J, glm::vec3(0.0f, pos[1], 0.0f));
		printf("%f ", pos[1]);
		
		pos[1] += dy * ddy;
		if (pos[1] >= 0.1f) {
			dy *= -1;
			
		}
		if (pos[1] < 0.0f) {
			dy *= -1;
			pos[1] = 0.0f;
			jm = false;
		}
		
	}
	void Turn()
	{
		R = glm::mat4(1.0f);
		

		switch (dir)
		{
		case 0:
			R = glm::rotate(R, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			angle = 180;
			rt = false;
			break;
		case 1:
			R = glm::rotate(R, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			angle = 0.0f;
			rt = false;
			break;
		case 2:
			R = glm::rotate(R, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			angle = 90.0f;
			rt = false;
			break;
		case 3:
			R = glm::rotate(R, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			angle = 270.0f;
			rt = false;
			break;
		}

		
	}

	void anglecheck(unsigned char key)
	{
		switch (key)
		{
		case 'w':
			if (dir != 0) {
				dir = 0;
				rt = true;
			}
			
			break;
		case 's':
			if (dir != 1) {
				dir = 1;
				rt = true;
			}

			break;
		case 'd':
			if (dir != 2) {
				dir = 2;
				rt = true;
			}
			break;
		case 'a':
			if (dir != 3) {
				dir = 3;
				rt = true;
			}
			break;
		}
	

	}

	void Move(int temp)
	{
		if (jm) Jump();
		if(angle == 360) angle =0;
		if (turn)TurnBack();
		if(rt)Turn();
		
		
		swing();
		
		T = glm::translate(T, glm::vec3(dx * 0.03f, 0.0f, dz*0.03f));
		pos[2] += dz * 0.03;
		pos[0] += dx * 0.03;
		
		
		
	}
	void Collison()
	{
		for (int i = 0; i < 3; i++)
		{
			if (pos[i] >= 0.9f || pos[i] <= -0.9f) {
				if(i == 2) dz *= -1;
				if (i == 0) dx *= -1;
				turn = true;
			}
		}
	}
	void Draw()
	{
		W = J * T * R;

		LG =  LGIT *  ALRX;
		LG2 =   LGIT2 * ALRX2;

		AM = AMIT * ALRX;
		AM2 = AMIT2 * ALRX2;

		BD = BDIT;

		HD = HDIT;

		NS = NSIT;

		for (int i = 0; i < RC; i++)
		{
			glBindVertexArray(vao[i]);
			if (i == 0)
			{
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(W*LG));
			}
			if (i == 1)
			{
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(W*LG2));
			}
			if (i == 2)
			{
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(W*BD));
			}
			if (i == 3)
			{
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(W*HD));
			}
			if (i == 4)
			{
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(W*AM));
			}
			if (i == 5)
			{
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(W*AM2));
			}

			if (i == 6)
			{
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(W*NS));
			}
			glDrawElements(GL_TRIANGLES, CI, GL_UNSIGNED_INT, 0);
		}
	}
};

struct Fitness {

	GLUquadric* qobj;

	glm::mat4 W = glm::mat4(1.0f);
	glm::mat4 R = glm::mat4(1.0f);
	glm::mat4 T = glm::mat4(1.0f);
	glm::mat4 T1 = glm::mat4(1.0f);
	glm::mat4 T2 = glm::mat4(1.0f);
	glm::mat4 T3 = glm::mat4(1.0f);
	glm::mat4 R3 = glm::mat4(1.0f);

	Fitness()
	{
		qobj = gluNewQuadric();
		R = glm::rotate(R, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		R3 = glm::rotate(R3, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		T = glm::translate(T, glm::vec3(0.0f, -SGSZ, 0.0f));
		T1 = glm::translate(T1, glm::vec3(0.7f, 0.35f, -0.3f));
		T2 = glm::translate(T2, glm::vec3(0.7f, 0.35f, 0.0f));
		T3 = glm::translate(T3, glm::vec3(0.7f, 0.35f, -0.325));
	}

	void draw_pole()
	{
		W = T * R;
		gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);

		glUniform3f(vColorLocation,NCL(0), NCL(255), NCL(0));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T1 * W ));
		gluCylinder(qobj, 0.01, 0.01, 0.35, 20, 8);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T2 * W));
		gluCylinder(qobj, 0.01, 0.01, 0.35, 20, 8);

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T3 * W * R3));
		glUniform3f(vColorLocation, NCL(255), NCL(0), NCL(0));
		gluCylinder(qobj, 0.015, 0.015, 0.35, 20, 8);
	}
};

glm::mat4 World = glm::mat4(1.0f);
glm::mat4 R = glm::mat4(1.0f);
glm::mat4 T = glm::mat4(1.0f);

glm::mat4 Rx = glm::mat4(1.0f);
glm::mat4 Ry = glm::mat4(1.0f);

glm::mat4 Tx = glm::mat4(1.0f);
glm::mat4 Ty = glm::mat4(1.0f);


float NCL(int c)
{
	return c / 255.0f;
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

Stage s;
Camera c;
Robot r;
Fitness f;

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

	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	glUseProgram(s_program);
}



void InitRT()
{


	R = glm::mat4(1.0f);

	Rx = glm::mat4(1.0f);
	Ry = glm::mat4(1.0f);


	
	//Rx = glm::rotate(Rx, glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0));
	//Ry = glm::rotate(Ry, glm::radians(-20.0f), glm::vec3(0.0, 1.0, 0.0));

	R = Rx * Ry;



}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(WIDTH, HEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Example"); // 윈도우 생성(윈도우 이름)
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
	//InitBuffer();

	s.InitBuffer();
	r.InitBuffer();
	InitRT();



	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard);
	glutSpecialFunc(ArrowKey);
	glutMainLoop(); // 이벤트 처리 시작 
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



	if (key == 'm') {
		if (w == false)w = true;
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

	if (key == 'z')
	{
		c.z++;
		c.CMTranslate();
	}

	if (key == 'Z')
	{
		c.z--;
		c.CMTranslate();
	}

	if (key == 'y')
	{
		c.CMRotate(1);
	}

	if (key == 'Y')
	{
		c.CMRotate(-1);
	}
	if (key == 'r')
	{
		c.CMRotate2(1);
	}
	if (key == 'R')
	{
		c.CMRotate2(-1);
	}
	
	if (key == 'w'|| key == 'a' || key == 's' || key == 'd')
	{
		if (key == 'w')
		{
			r.dx = 0;
			r.dz = -1;
		}
		if (key == 'a')
		{
			r.dx = -1;
			r.dz = 0;
		}
		if (key == 's')
		{
			r.dx = 0;
			r.dz = 1;
		}
		if (key == 'd')
		{
			r.dx = 1;
			r.dz = 0;
		}
		r.anglecheck(key);
		
	}
	if (key == 'j' || key  == 'J')
	{
		jm = true;
	}
	if (key == 'i' || key == 'I')
	{
		r.init();
		c.init();
	}

	if ( key == 'M' || key == 't' || key == 'o' || key == 'O')
	{
		if (stop == true)
		{
			stop = false;
			glutTimerFunc(t, TimerFunction, 1);
		}

		if (s.angle <= 0.0f && s.angle >= -90.0f)
		{
			if (key == 'o')
			{
				df = -1;
			}
			if (key == 'O')
			{
				df = 1;
			}
		}

		
		
	}
	/*if (key == 's' || key == 'S')
	{
		stop = true;
	}*/
	
	if (key == 'q' || key == 'Q')
	{
		exit(0);
	}
	glutPostRedisplay();
}

void Rotate()
{
	
}
void Translate()
{
}



GLvoid TimerFunction(int value)
{
	if (stop == true) return;

	s.Update();
	r.Collison();
	r.Move(temp);
	
	glutPostRedisplay();
	glutTimerFunc(t, TimerFunction, 1);
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	glUseProgram(s_program);
	

	c.cameraset();
	

	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 50.0f);
	projection = projection * projTZ * projTX * projRY;
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");

	
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	if (!w)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (h)glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
	
	modelLocation = glGetUniformLocation(s_program, "modelTransform");


	//스자이공부


	World = R * T;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(World));

	
	vColorLocation = glGetUniformLocation(s_program, "outcolor");
	flagLocation = glGetUniformLocation(s_program, "flag");
	glUniform1i(flagLocation, 1);
	s.Draw();

	f.draw_pole();

	flagLocation = glGetUniformLocation(s_program, "flag");
	glUniform1i(flagLocation, 0);
	r.Draw();

	

	glutSwapBuffers(); // 화면에 출력하기

}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}