#include "source.h"
const int TIMER_TICK = 10;
void drawScene();
void convertDeviceXYOpenGLXY(int x, int y, float*, float*);
void InitBuffer();
void InitBuffer2();
void Keyboard(unsigned char key, int x, int y);
unsigned int ShapeVAO[2], PyramidVAO;
float Angle = 20.0f;
float upangle = 0.0f;
float Frontupangle = 0.0f;
void TimerFunc(int value);

bool rotates = false; 
bool Frontrotates = false;

bool cubes = false; 


GLuint RightVertexArrayobjectID; 
GLuint LeftVertexArrayobjectID;
GLuint UpVertexArrayobjectID;
GLuint DownVertexArrayobjectID;
GLuint BackVertexArrayobjectID;
GLuint FrontVertexArrayobjectID;

GLuint RightVertexBufferobjectID[2];
GLuint LeftVertexBufferobjectID[2];
GLuint UpVertexBufferobjectID[2];
GLuint DownVertexBufferobjectID[2];
GLuint BackVertexBufferobjectID[2];
GLuint FrontVertexBufferobjectID[2];

GLuint RightIndexBufferobjectID;
GLuint LeftIndexBufferobjectID;
GLuint UpIndexBufferobjectID;
GLuint DownIndexBufferobjectID;
GLuint BackIndexBufferobjectID;
GLuint FrontIndexBufferobjectID;

void main(int argc, char** argv)
{
	glutInit(&argc, argv); //glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //디스플레이 모드 설정
	glutInitWindowPosition(100, 100); //윈도우의 위치 지정
	glutInitWindowSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT); //윈도우의 크기 지정
	glutCreateWindow("using index buffer");//윈도우 생성

	//[ GLEW 초기화 하기 ]=======================================================
	glewExperimental = GL_TRUE; //glew 초기화
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized\n";

	glutTimerFunc(TIMER_TICK, TimerFunc, 1);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	//glutMouseFunc(MouseEvent);
	//glutReshapeFunc (); // 다시 그리기 함수 지정 
	glutMainLoop();
}


GLfloat LeftShape[] =
{
-0.5f, -0.5f, -0.5f,  -0.5f, -0.5f, +0.5f, 
-0.5f, +0.5f, -0.5f, -0.5f, +0.5f, +0.5f
};

GLfloat RightShape[] =
{
	+0.5f, -0.5f, -0.5f,  +0.5f, -0.5f, +0.5f,
+0.5f, +0.5f, -0.5f,  +0.5f, +0.5f, +0.5f
};

GLfloat UpShape[] =
{
 -0.5f, +0.5f, -0.5f,
-0.5f, +0.5f, +0.5f, 
+0.5f, +0.5f, -0.5f, 
+0.5f, +0.5f, +0.5f
};

GLfloat DownShape[] =
{
-0.5f, -0.5f, -0.5f,  -0.5f, -0.5f, +0.5f, 
+0.5f, -0.5f, -0.5f,  +0.5f, -0.5f, +0.5f
};

GLfloat FrontShape[] =
{
-0.5f, -0.5f, +0.5f, -0.5f, +0.5f, +0.5f, +0.5f, -0.5f, +0.5f,
  +0.5f, +0.5f, +0.5f
};

GLfloat BackShape[] =
{
-0.5f, -0.5f, -0.5f,  -0.5f, +0.5f, -0.5f,
 +0.5f, -0.5f, -0.5f, +0.5f, +0.5f, -0.5f
};

GLfloat Pyramid[] =
{
  +0.0f, +0.5f, +0.0f,   //0 
  +0.5f, -0.5f, -0.5f,   //
  +0.5f, -0.5f, +0.5f,   //
  -0.5f, -0.5f, +0.5f,   //
  -0.5f, -0.5f, -0.5f    // 
};

unsigned int Right_Index[] =
{ 1,0,2 };
unsigned int Left_Index[] =
{ 4, 0, 3 };
unsigned int Back_Index[] =
{ 0,4,1  };
unsigned int Front_Index[] =
{ 3, 0, 2 };
unsigned int Down_Index[] =
{ 1,2,3, 1, 3,4 };


GLfloat Axes[] =
{ /*X축*/ +1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f,
/*Y축*/  +0.0f, +1.0f, +0.0f, +0.0f, -1.0f, +0.0f,
/*Z축*/  +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, -1.0f };

unsigned int Axes_Index[] = { 0, 1 , 2, 3 , 4, 5 };

unsigned int Leftindex[] =
{0,2,1,  1,2,3/*왼쪽*/ };
unsigned int Backindex[] =
{ 0, 2, 1, 1, 2, 3/*뒷쪽*/ };
unsigned int Frontindex[] =
{ 0,1,2,2,1,3 /*앞쪽*/ };
unsigned int Upindex[] =
{ 0,2,1, 2, 3,1 /*위쪽*/ };
unsigned int Rightindex[] =
{ 0,1,2,2,1,3  /*오른쪽*/ };
unsigned int Downindex[] =
{ 3,2,1, 1, 2, 0/*아래쪽*/ };


GLfloat LeftColors[] = { 1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }; //
GLfloat BackColors[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
GLfloat RightColors[] = { 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat UpColors[] = { 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f };
GLfloat DownColors[] = { 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.5f,  0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 0.5f };
GLfloat FrontColors[] = { 0.0f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f,  0.0f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f };

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

void drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CW);
	//glCullFace(GL_BACK);

	//if (culling == true) 
	//glEnable(GL_CULL_FACE); //이거 해야지 다 보임
	//if (culling == false) glDisable(GL_CULL_FACE);

	//glDisable(GL_CULL_FACE);
	glUseProgram(Complie_Shaders());

	//---버텍스세이더에서각변환행렬값을받아온다. 
	int modelLoc = glGetUniformLocation(Complie_Shaders(), "model");
	int viewLoc = glGetUniformLocation(Complie_Shaders(), "view");
	int projLoc = glGetUniformLocation(Complie_Shaders(), "projection");

	glm::vec3 cameraPos = glm::vec3(0.0f, 1.5f, 3.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 scaleMatrix = glm::mat4(1.0f); // 단위행렬로초기화 
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(1.0, 1.0, 1.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(scaleMatrix));

	glBindVertexArray(ShapeVAO[0]);
	InitBuffer2();
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	if (cubes == true)
	{
		InitBuffer();
		glBindVertexArray(BackVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//InitBuffer();
		glBindVertexArray(LeftVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(RightVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(DownVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		if (Frontrotates == true)
		{
			model = glm::translate(model, glm::vec3(0.0f, -0.5f, +0.5f));
			model = glm::rotate(model, glm::radians(Frontupangle), glm::vec3(1.0f, 0.0, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, +0.5f, -0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		glBindVertexArray(FrontVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		if (Frontrotates == true)
		{
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}


		model = glm::translate(model, glm::vec3(0.0f, +0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(upangle), glm::vec3(1.0f, 0.0, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, +0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(UpVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	if (cubes == false)
	{
		InitBuffer();
	
		glBindVertexArray(DownVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		///////////////////////////////////////////////////////////////////////////////////////////////////
		if (Frontrotates == true)
		{
			model = glm::translate(model, glm::vec3(0.0f, -0.5f, -0.5f));
			model = glm::rotate(model, glm::radians(-Frontupangle), glm::vec3(1.0f, 0.0, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, +0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		glBindVertexArray(BackVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		if (Frontrotates == true)
		{
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////
		if (Frontrotates == true)
		{
			model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
			model = glm::rotate(model, glm::radians(Frontupangle), glm::vec3(0.0f, 0.0, 1.0f));
			model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		glBindVertexArray(LeftVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		if (Frontrotates == true)
		{
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////		
		if (Frontrotates == true)
		{
			model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));
			model = glm::rotate(model, glm::radians(-Frontupangle), glm::vec3(0.0f, 0.0, 1.0f));
			model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		glBindVertexArray(RightVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		if (Frontrotates == true)
		{
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////
		{
			if (Frontrotates == true)
			{
				model = glm::translate(model, glm::vec3(0.0f, -0.5f, +0.5f));
				model = glm::rotate(model, glm::radians(Frontupangle), glm::vec3(1.0f, 0.0, 0.0f));
				model = glm::translate(model, glm::vec3(0.0f, +0.5f, -0.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			}
			glBindVertexArray(FrontVertexArrayobjectID);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			if (Frontrotates == true)
			{
				model = glm::mat4(1.0f);
				model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			}

		}
		model = glm::translate(model, glm::vec3(0.0f, +0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(upangle), glm::vec3(1.0f, 0.0, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, +0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(UpVertexArrayobjectID);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	glutPostRedisplay();

	glutSwapBuffers();
}

void InitBuffer()
{
	glGenVertexArrays(1, &RightVertexArrayobjectID);
	glGenVertexArrays(1, &LeftVertexArrayobjectID);
	glGenVertexArrays(1, &UpVertexArrayobjectID);
	glGenVertexArrays(1, &DownVertexArrayobjectID);
	glGenVertexArrays(1, &FrontVertexArrayobjectID);
	glGenVertexArrays(1, &BackVertexArrayobjectID);

	glGenBuffers(2, RightVertexBufferobjectID);
	glGenBuffers(2, LeftVertexBufferobjectID);
	glGenBuffers(2, UpVertexBufferobjectID);
	glGenBuffers(2, DownVertexBufferobjectID);
	glGenBuffers(2, BackVertexBufferobjectID);
	glGenBuffers(2, FrontVertexBufferobjectID);

	glGenBuffers(1, &LeftIndexBufferobjectID);
	glGenBuffers(1, &BackIndexBufferobjectID);
	glGenBuffers(1, &DownIndexBufferobjectID);
	glGenBuffers(1, &FrontIndexBufferobjectID);
	glGenBuffers(1, &UpIndexBufferobjectID);
	glGenBuffers(1, &RightIndexBufferobjectID);

	if (cubes == true)
	{
		glBindVertexArray(LeftVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, LeftVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(LeftShape), LeftShape, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LeftIndexBufferobjectID); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Leftindex), Leftindex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, LeftVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(LeftColors), LeftColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(BackVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, BackVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(BackShape), BackShape, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BackIndexBufferobjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Backindex), Backindex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, BackVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(BackColors), BackColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(FrontVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, FrontVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FrontShape), FrontShape, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FrontIndexBufferobjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Frontindex), Frontindex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, FrontVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FrontColors), FrontColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(UpVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, UpVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(UpShape), UpShape, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, UpIndexBufferobjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Upindex), Upindex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, UpVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(UpColors), UpColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(DownVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, DownVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(DownShape), DownShape, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DownIndexBufferobjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Downindex), Downindex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, DownVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(DownColors), DownColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(RightVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, RightVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RightShape), RightShape, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RightIndexBufferobjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Rightindex), Rightindex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, RightVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RightColors), RightColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
	}

	if (cubes == false)
	{
		glBindVertexArray(LeftVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, LeftVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Pyramid), Pyramid, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LeftIndexBufferobjectID); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Left_Index), Left_Index, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, LeftVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(LeftColors), LeftColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(BackVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, BackVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Pyramid), Pyramid, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BackIndexBufferobjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Back_Index), Back_Index, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, BackVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(BackColors), BackColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(FrontVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, FrontVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Pyramid), Pyramid, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FrontIndexBufferobjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Front_Index), Front_Index, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, FrontVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FrontColors), FrontColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(DownVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, DownVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Pyramid), Pyramid, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DownIndexBufferobjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Down_Index), Down_Index, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, DownVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(DownColors), DownColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(RightVertexArrayobjectID);
		glBindBuffer(GL_ARRAY_BUFFER, RightVertexBufferobjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Pyramid), Pyramid, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RightIndexBufferobjectID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Right_Index), Right_Index, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, RightVertexBufferobjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RightColors), RightColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
	}

}

void InitBuffer2()
{
	glGenVertexArrays(1, &ShapeVAO[1]);
	GLuint  VBO[2], EBO;
	glGenBuffers(2, VBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Axes), Axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Axes_Index), Axes_Index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LeftColors), LeftColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

int ModeCounter = 0;
int CullingMode = 0;

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'y') { Angle += 20.f; glutPostRedisplay(); }
	if (key == 'Y') { Angle -= 20.f; glutPostRedisplay(); }

	if (key == 'f') { rotates = true; }
	if (key == 'F') { rotates = false;  }

	if (key == 't') { 
		Frontupangle = 0.0f; 
			Frontrotates = true; }
	if (key == 'T') { Frontrotates = false; }

	if (key == 'c')
	{
		cubes = true;
	}
	glutPostRedisplay();
}


void TimerFunc(int value)
{
	if( rotates==false )
		upangle += 0.0f;
	else 
	upangle += 5.0f;
	

	if (Frontrotates == false)
	{
		//if (Frontupangle == 0.0f)
		//	Frontupangle = 0;
	}
	else
	{

		if (Frontupangle <= 0.0f|| Frontupangle <= 90.0f)
			Frontupangle += 2.0f;
	}
	glutPostRedisplay();
	glutTimerFunc(TIMER_TICK, TimerFunc, 1);
}