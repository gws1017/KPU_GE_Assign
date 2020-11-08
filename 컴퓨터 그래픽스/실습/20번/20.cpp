#include <iostream>
#include <random>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm.hpp>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp>

using namespace std;

//윈도우 크기
#define WIDTH 500
#define HEIGHT 500

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);

glm::mat4 lt = glm::mat4(1.0f);
glm::vec3 ltPos = glm::vec3(0.0f, 0.5f, 1.0f);
glm::vec4 ltPos4 = glm::vec4(ltPos,1.0f);
glm::mat4 ltRy = glm::mat4(1.0f);
glm::mat4 ltT = glm::mat4(1.0f);

glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::vec3 LC = glm::vec3(1.0f, 1.0f, 1.0f);

GLvoid TimerFunction(int value);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

GLchar *vertexsource, *fragmentsource;
GLuint vertexshader, fragmentshader;
GLuint s_program;

unsigned int modelLocation, projectionLocation, viewLocation, lightPosLocation, lightColorLocation, viewposLocation, vColorLocation, flagLocation;

GLuint lc = 0;

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
bool loadOBJ(
	const char * path,
	std::vector < glm::vec3 > & out_vertices,
	std::vector < glm::vec2 > & out_uvs,
	std::vector < glm::vec3 > & out_normals
)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;



	char bind[100];

	FILE *fptr = NULL;

	fopen_s(&fptr, "sphere.obj", "r");

	if (fptr == NULL)
	{
		cout << "파일을 못찾음" << endl;
	}

	else {
		cout << "파일찾음" << endl;
	}


	while (!feof(fptr))
	{
		fscanf(fptr, "%s", bind);
		if (bind[0] == 'v' && bind[1] == 'n' && bind[2] == '\0')
		{
			glm::vec3 normal;
			fscanf(fptr, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0')
		{
			glm::vec2 uv;
			fscanf(fptr, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (bind[0] == 'v' && bind[1] == '\0') {
			glm::vec3 vertex;

			fscanf(fptr, "%f %f %f",
				&vertex.x, &vertex.y,
				&vertex.z);

			temp_vertices.push_back(vertex);
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(fptr, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}


	}


	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}
	fclose(fptr);
}

struct sphere {
	std::vector < glm::vec3 > vertices;
	std::vector < glm::vec2 > uv;
	std::vector < glm::vec3 > normals;

	GLuint vao, vbo[2];

	glm::mat4 S1 = glm::mat4(1.0f);
	glm::mat4 S2 = glm::mat4(1.0f);
	glm::mat4 S3 = glm::mat4(1.0f);
	glm::mat4 S4 = glm::mat4(1.0f);
	glm::mat4 T2 = glm::mat4(1.0f);
	glm::mat4 T3 = glm::mat4(1.0f);
	glm::mat4 Ry = glm::mat4(1.0f);
	
	void LoadOBJ()
	{
		loadOBJ("sphere", vertices, uv, normals);
	}

	void InitBuffer()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(2, vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1 ,3, GL_FLOAT, GL_FALSE, 12, 0);
		glEnableVertexAttribArray(1);

		
		S1 = glm::scale(S1, glm::vec3(0.009,0.009,0.009));
		S2 = glm::scale(S2, glm::vec3(0.005,0.005,0.005));
		S3 = glm::scale(S3, glm::vec3(0.003,0.003,0.003));
		S4 = glm::scale(S4, glm::vec3(0.01, 0.01, 0.01));
		T2 = glm::translate(T2, glm::vec3(-0.6,0.0,0.0));
		T3 = glm::translate(T3, glm::vec3(-1.0, 0.0, 0.0));

		
		ltT = glm::translate(ltT, ltPos);
	}

	void Rotate(int dy)
	{
		Ry = glm::rotate(Ry, glm::radians(dy * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	void draw(int i)
	{
		ltRy = glm::mat4(1.0f);
		glBindVertexArray(vao);
		glUniform1i(flagLocation, 0);
		if (i == 0)
		{
			glUniform3f(vColorLocation, 0.8f, 0.0f, 0.0f);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(S1));
		}
		if (i == 1)
		{
			glUniform3f(vColorLocation, 0.0f, 0.8f, 0.0f);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T2 * S2));
		}
		if (i == 2)
		{
			glUniform3f(vColorLocation, 0.0f, 0.0f, 0.8f);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(T3 * S3));
		}
		if (i == 3)
		{

			glUniform1i(flagLocation, 1);
			glUniform3f(vColorLocation, 1.0f, 1.0f, 1.0f);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Ry * ltT  * S4));
		}
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(glm::vec3));
	}
};	

sphere s[4];


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

	for (int i = 0; i < 4; i++)
	{
		s[i].LoadOBJ();
		s[i].InitBuffer();
	}
	
	



	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard);
	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid KeyBoard(unsigned char key, int x, int y)
{
	if (key == 'c' || key == 'C')
	{
		if (lc == 0) {
			lc = 1;
			LC = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else if (lc == 1) {
			lc = 2;
			LC = glm::vec3(0.5f, 0.3f, 0.3f);
		}
		else if (lc == 2) {
			lc = 0;
			LC = glm::vec3(0.3f, 0.5f, 0.3f);
		}
	}
	
	if (key == 'r')
	{
		s[3].Rotate(1);
		ltRy = glm::rotate(ltRy, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ltPos4 = ltPos4 * ltRy;
	}
	if (key == 'R')
	{
		s[3].Rotate(-1);
		ltRy = glm::rotate(ltRy, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ltPos4 = ltPos4 * ltRy;
	}
	glutPostRedisplay();
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	glUseProgram(s_program);

	vColorLocation = glGetUniformLocation(s_program, "Colors");
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	viewLocation = glGetUniformLocation(s_program, "viewTransform");
	viewposLocation = glGetUniformLocation(s_program, "viewPos");
	lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	flagLocation = glGetUniformLocation(s_program, "flag");
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 50.0f);
	
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	glUniform3f(viewposLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	glUniform3f(lightPosLocation, ltPos4.x, ltPos4.y, ltPos4.z);

	if (lc == 0) glUniform3f(lightColorLocation, LC.x, LC.y, LC.z);
	if (lc == 1) glUniform3f(lightColorLocation, LC.x, LC.y, LC.z);
	if (lc == 2) glUniform3f(lightColorLocation, LC.x, LC.y, LC.z);

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 0; i < 4; i++)
	{
		s[i].draw(i);
	}
	

	



	glutSwapBuffers(); // 화면에 출력하기

}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
