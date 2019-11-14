#include "source.h"
void drawScene();
void convertDeviceXYOpenGLXY(int x, int y, float*, float*);
void InitBuffer();
void InitBuffer2();
void Keyboard(unsigned char key, int x, int y);
unsigned int ShapeVAO[2], PyramidVAO;
bool Tri = true, Pyra = false;
bool PolyModeFILL = true;
float Angle = 20.0f;
bool culling = true; 
bool cullingNO = false;

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

	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	//glutMouseFunc(MouseEvent);
	//glutReshapeFunc (); // 다시 그리기 함수 지정 
	glutMainLoop();
}


GLfloat TriShape[] =
{
-0.5f, -0.5f, -0.5f,  -0.5f, -0.5f, +0.5f, -0.5f, +0.5f, -0.5f,
-0.5f, +0.5f, +0.5f, +0.5f, -0.5f, -0.5f,  +0.5f, -0.5f, +0.5f,
+0.5f, +0.5f, -0.5f,  +0.5f, +0.5f, +0.5f
};

GLfloat Pyramid[] =
{
  +0.0f, +1.0f, +0.0f, +0.5f, +0.0f, -0.5f, +0.5f, +0.0f, +0.5f,
  -0.5f, +0.0f, +0.5f, -0.5f, +0.0f, -0.5f
};


GLfloat Axes[] =
{ /*X축*/ +1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f,
/*Y축*/  +0.0f, +1.0f, +0.0f, +0.0f, -1.0f, +0.0f,
/*Z축*/  +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, -1.0f };

unsigned int Axes_Index[] = { 0, 1 , 2, 3 , 4, 5 };

unsigned int index[] =
{ 2,6,4, 0,2,4, 5,6,7,5,4,6,2,7,6,2,3,7,3,2,1,2,0,1,5,7,3,5,3,1,0,4,5,0,5,1};

unsigned int Pyramid_Index[] =
{ 1,3,4, 1,2,3, 0,4,1, 0,1,2, 0,3,4, 0,2,3 };

GLfloat Colors[] = { 1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f , 1.0f ,0.0f,
								1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.2f, 0.2f, 1.0f,  0.0f , 0.5f ,1.0f };

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
	glCullFace(GL_BACK);

	if(culling==true) glEnable(GL_CULL_FACE);
	if (culling == false) glDisable(GL_CULL_FACE);

	//glDisable(GL_CULL_FACE);
	glUseProgram(Complie_Shaders());

	//---버텍스세이더에서각변환행렬값을받아온다. 
	int modelLoc = glGetUniformLocation(Complie_Shaders(), "model");
	int viewLoc = glGetUniformLocation(Complie_Shaders(), "view");
	int projLoc = glGetUniformLocation(Complie_Shaders(), "projection");

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
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





	glBindVertexArray(ShapeVAO[0]); // VAO를 바인드하기
	//glBindVertexArray(PyramidVAO);

	//---모델링변환, 뷰잉변환, 투영변환행렬을설정한후, 버텍스세이더에저장한다.
    view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	//for (unsigned int i = 0; i < 10; ++i)
	//{
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, cubePositions[i]);
	model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	InitBuffer();

	if (PolyModeFILL == false)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (PolyModeFILL == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//}
	//glDrawArrays(GL_TRIANGLES, 0, 36);  // 육면체: 6개의 면 x 2개의 삼각형 x 3개의 정점
	glutSwapBuffers();
}

void InitBuffer()
{
	glGenVertexArrays(1 ,&ShapeVAO[0]);
	GLuint  ShapeVBO[2], ShapeEBO;
	glGenBuffers(2, ShapeVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &ShapeEBO);
	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[0]);

	if (Tri == true && Pyra == false)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriShape), TriShape, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ShapeEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
	}
	if (Tri == false && Pyra == true)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(Pyramid), Pyramid, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ShapeEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Pyramid_Index), Pyramid_Index, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void convertDeviceXYOpenGLXY(int x, int y, float* glx, float* gly)
{
	int w = WINDOW_SIZE_WIDTH;
	int h = WINDOW_SIZE_HEIGHT;

	*glx = float(x - (float)w / 2.0)* float(1.0 / (float)(w / 2.0));
	*gly = -float(y - (float)h / 2.0)* float(1.0 / (float)(h / 2.0));
}
int ModeCounter = 0;
int CullingMode = 0;
void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'y') { Angle += 20.f; glutPostRedisplay(); }
	if (key == 'Y') { Angle -= 20.f; glutPostRedisplay(); }

	if (key == 'c') { Tri = true, Pyra = false; glutPostRedisplay(); }
	if (key == 'p') { Tri = false, Pyra = true; glutPostRedisplay(); }
	if (key == 'w' || key == 'W')
	{
		if (ModeCounter == 2)
		{
			ModeCounter = 0;
		}
		if (ModeCounter)
			PolyModeFILL = false;
		else PolyModeFILL = true;

		++ModeCounter;
		glutPostRedisplay();
	}

	if (key == 'h') 
	{
		if (CullingMode == 2)
		{
			CullingMode = 0;
		}
		if (CullingMode)
			culling = false;
		else culling = true;

		++CullingMode;
		glutPostRedisplay();
	}
}