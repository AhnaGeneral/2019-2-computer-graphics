#include "source.h"

void drawScene();
void Keyboard(unsigned char key, int x, int y);
void InitBuffer();
void InitBuffer2();
void InitBuffer3();
void InitBuffer4();

unsigned int First_RectVAO, Second_RectVAO,Third_RectVAO,Ground_RectVAO;

int modelnumber = 1;
float RotateY = 0.0f;
float RotateX = 0.0f;
float RotateZ = 0.0f;
float CameraX = 0.0f;
float CameraZ = 0.0f;
float CameraY = 0.0f;
float angle = 0.0f; 
float fAngle = 0.0f; 
float radius = 5.0f;

float CenterXRotate = 0.0f;
float CenterZRotate = 0.0f;
bool MovingX = false;
bool MovingY = false;
bool MovingZ = false;
bool Movingx = false;
bool Movingy = false;
bool Movingz = false;
bool RotateX_Check = false;
bool RotateZ_Check = false;

void DefaultTimerFunction(int);

GLfloat Ground_Rect[] =
{
-10.0f, -0.2f, +10.0f,
-10.0f, -0.2f, -10.0f,
+10.0f, -0.2f, -10.0f,
+10.0f, -0.2f, +10.0f,

};
GLfloat First_Rect[] =
{
-0.5f, -0.2f, +0.5f,
-0.5f, -0.2f, -0.5f,
+0.5f, -0.2f, -0.5f,
+0.5f, -0.2f, +0.5f,
-0.5f, +0.2f, +0.5f,
-0.5f, +0.2f, -0.5f,
+0.5f, +0.2f, -0.5f,  
+0.5f, +0.2f, +0.5f
};
GLfloat Second_Rect[] =
{
-0.3f, -0.0f, +0.3f,
-0.3f, -0.0f, -0.3f,
+0.3f, -0.0f, -0.3f,
+0.3f, -0.0f, +0.3f,
-0.3f, +0.3f, +0.3f,
-0.3f, +0.3f, -0.3f,
+0.3f, +0.3f, -0.3f,
+0.3f, +0.3f, +0.3f
};
GLfloat Third_Rect[] =
{
-0.1f, +0.0f, +0.1f,
-0.1f, +0.0f, -0.1f,
+0.1f, +0.0f, -0.1f,
+0.1f, +0.0f, +0.1f,
-0.1f, +0.5f, +0.1f,
-0.1f, +0.5f, -0.1f,
+0.1f, +0.5f, -0.1f,
+0.1f, +0.5f, +0.1f
};
unsigned int Ground_Index[] =
{ 0,1,2,
2,3,0
};
unsigned int First_Rect_Index[] =
{ 0,4,3,
3,4,7,
0,2,1,
3,2,1,
5,6,7,
5,7,4,
2,6,5,
2,5,1,
1,5,4,
1,4,0,
3,7,6,
3,6,2};

unsigned int Second_Rect_Index[] =
{ 0,4,3,
3,4,7,
0,2,1,
3,2,1,
5,6,7,
5,7,4,
2,6,5,
2,5,1,
1,5,4,
1,4,0,
3,7,6,
3,6,2 };
unsigned int Third_Rect_Index[] =
{ 0,4,3,
3,4,7,
0,2,1,
3,2,1,
5,6,7,
5,7,4,
2,6,5,
2,5,1,
1,5,4,
1,4,0,
3,7,6,
3,6,2 };
GLfloat Ground_Colors[] =
{ +0.7f, +0.5f, +0.0f,
  +0.7f, +0.5f, +0.0f,
  +0.7f, +0.5f, +0.0f,
  +0.7f, +0.5f, +0.0f,
  +0.7f, +0.5f, +0.0f,

};
GLfloat First_Rect_Colors[] =
{ +1.0f, +0.0f, +0.0f, 
  +1.0f, +0.0f, +0.0f,
  +1.0f, +0.0f, +0.0f, 
  +1.0f, +0.0f, +0.0f,
  +1.0f, +0.0f, +0.0f, 
  +1.0f, +0.0f, +0.0f,
 +1.0f, +0.0f, +0.0f,
  +1.0f, +0.0f, +0.0f,
  +1.0f, +0.0f, +0.0f,
  +1.0f, +0.0f, +0.0f,
  +1.0f, +0.0f, +0.0f,
  +1.0f, +0.0f, +0.0f

};
GLfloat Second_Rect_Colors[] =
{ 
	 +0.0f, +1.0f, +0.0f,
  +0.0f, +1.0f, +0.0f,
  +0.0f, +1.0f, +0.0f,
  +0.0f, +1.0f, +0.0f,
  +0.0f, +1.0f, +0.0f,
  +0.0f, +1.0f, +0.0f,
 +0.0f, +1.0f, +0.0f,
  +0.0f, +1.0f, +0.0f,
  +0.0f, +1.0f, +0.0f,
  +0.0f, +1.0f, +0.0f,
  +0.0f, +1.0f, +0.0f,
  +0.0f, +1.0f, +0.0f
};
GLfloat Third_Rect_Colors[] =
{ 
	 +0.0f, +0.0f, +1.0f,
  +0.0f, +0.0f, +1.0f,
  +0.0f, +0.0f, +1.0f,
  +0.0f, +0.0f, +1.0f,
  +0.0f, +0.0f, +1.0f,
  +0.0f, +0.0f, +1.0f,
 +0.0f, +0.0f, +1.0f,
  +0.0f, +0.0f, +1.0f,
  +0.0f, +0.0f, +1.0f,
  +0.0f, +0.0f, +1.0f,
  +0.0f, +0.0f, +1.0f,
  +0.0f, +0.0f, +1.0f
};


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
	//glutMouseFunc(MouseEvent);
	glutTimerFunc(1, DefaultTimerFunction, 0);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutMainLoop();
}

void drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(Complie_Shaders());

	if (RotateY == 360.0f|| RotateY == -360.0f) RotateY = 0.0f;
	
	if (Movingx == true) {
		if (RotateX_Check == false) {
			RotateX += 1.0f;
			if (RotateX > 89.0f) { RotateX_Check = true; }
		}
		else if (RotateX_Check == true) {
			RotateX -= 1.0f;
			if (RotateX < 0.1f) { RotateX_Check = false;  }
		}	
	}
	if (MovingX == true) {
		if (RotateX_Check == false) {
			RotateX -= 1.0f;
			if (RotateX < -89.0f) { RotateX_Check = true; }
		}
		else if (RotateX_Check == true) {
			RotateX += 1.0f;
			if (RotateX > -0.1f) { RotateX_Check = false; }
		}
	}
	if (Movingz == true) {
		if (RotateZ_Check == false) {
			RotateZ += 1.0f;
			if (RotateZ > 89.0f) { RotateZ_Check = true; }
		}
		else if (RotateZ_Check == true) {
			RotateZ -= 1.0f;
			if (RotateZ < 0.1f) { RotateZ_Check = false; }
		}
	}

	if (MovingZ == true) {
		if (RotateZ_Check == false) {
			RotateZ -= 1.0f;
			if (RotateZ < -89.0f) { RotateZ_Check = true; }
		}
		else if (RotateZ_Check == true) {
			RotateZ += 1.0f;
			if (RotateZ > -0.1f) { RotateZ_Check = false; }
		}
	}
	if (MovingY == true) RotateY += 1.0f;
	else if (Movingy == true)RotateY -= 1.0f;
	//float radius = 5.0f;
	float camX = sin(fAngle) * radius;
	float camZ = cos(fAngle) * radius;
	
	glm::vec3 cameraPos = glm::vec3(0.0f+CameraX+ camX, 0.0f, 0.0f+ CameraZ+camZ);
	glm::vec3 cameraDirection = glm::vec3(0.0f + CameraX, 1.0f, 0.0f);
	//glm::rotate(cameraDirection, cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)); 
	glm::vec3 cameraUp = glm::vec3(0.0f , 1.0f,  0.0f);

	int modelLoc = glGetUniformLocation(Complie_Shaders(), "model");
	int viewLoc = glGetUniformLocation(Complie_Shaders(), "view");
	int projLoc = glGetUniformLocation(Complie_Shaders(), "projection");


	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 Matrix = glm::mat4(1.0f);
	Matrix = glm::scale(Matrix, glm::vec3(3.0, 3.0, 3.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Matrix));



	////[ 땅 ]/////////////////////
	glBindVertexArray(Ground_RectVAO);
	InitBuffer4();
	glm::mat4 modeltrans = glm::mat4(1.0f);
	modeltrans = glm::translate(modeltrans, glm::vec3(0.0f, -2.0f, 0.0f));
	modeltrans = glm::rotate(modeltrans,angle, glm::vec3(0.0f, 1.0f, 0.0f)); 
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeltrans));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//[ 첫번째 사각형 ]/////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(First_RectVAO);
	InitBuffer();
	glm::mat4 model1trans = modeltrans;

	//model1trans = glm::scale(model1trans, glm::vec3(3.0, 3.0, 3.0));
	model1trans = glm::rotate(model1trans, glm::radians(RotateY), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1trans));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	////[두번째 사각형]//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(Second_RectVAO);
	InitBuffer2();
	glm::mat4 model2trans = model1trans;
	model2trans = glm::translate(model2trans, glm::vec3(0.0f, 0.3f, 0.0f));
	model2trans = glm::rotate(model2trans, glm::radians(RotateX), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2trans));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	////[세번째 사각형]////////////////////////////
	glBindVertexArray(Third_RectVAO);
	InitBuffer3();
	glm::mat4 model3trans = model2trans;
	model3trans = glm::translate(model3trans, glm::vec3(0.0f, 0.25f, 0.0f));
	model3trans = glm::rotate(model3trans, glm::radians(RotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3trans));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

int ModeCounter = 0;
int ModeRotate = 0;
int changecounter = 0;

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'b') { Movingy = true; MovingY = false; }
	if (key == 'B') { Movingy = false; MovingY = true; }
	if (key == 'S'|| key == 's') {
		MovingX = false;
		Movingx = false;
		MovingY = false;
		Movingy = false;
		MovingZ = false;
		Movingz = false;
	}
	if (key == 'm') { Movingx = true; MovingX = false; }
	if (key == 'M') { Movingx = false; MovingX = true; }
	if (key == 't') { Movingz = true; MovingZ = false; }
	if (key == 'T') { Movingz = false; MovingZ = true; }

	if (key == 'C' || key == 'c') { RotateX = 0; RotateY = 0; RotateZ = 0;
	angle += 1; 
	};

	if (key == 'z') { CameraZ -= 1.0f; }
	if (key == 'Z') { CameraZ += 1.0f; }
	if (key == 'x') { CameraX -= 1.0f; }
	if (key == 'X') { CameraX += 1.0f; }
	if (key == 'r') { fAngle -= 0.5f; 	 /*radius += 5.0f;*/
	}
	if (key == 'R') { fAngle += 0.5f; 	 /*radius += 5.0f;*/
	}

	glutPostRedisplay();
}

void DefaultTimerFunction(int value)
{
	glutTimerFunc(1, DefaultTimerFunction, 0); // 타이머함수재설정
	glutPostRedisplay(); // 화면재출력
}

void InitBuffer()
{
	glGenVertexArrays(1, &First_RectVAO);
	GLuint  ShapeVBO[2], ShapeEBO;
	glGenBuffers(2, ShapeVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &ShapeEBO);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(First_Rect), First_Rect, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ShapeEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(First_Rect_Index), First_Rect_Index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(First_Rect_Colors), First_Rect_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

}
void InitBuffer2()
{
	glGenVertexArrays(1, &Second_RectVAO);
	GLuint  PyramidVBO[2], PyramidEBO;
	glGenBuffers(2, PyramidVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &PyramidEBO);
	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Second_Rect), Second_Rect, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PyramidEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Second_Rect_Index), Second_Rect_Index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Second_Rect_Colors), Second_Rect_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}
void InitBuffer3()
{
	glGenVertexArrays(1, &Third_RectVAO);
	GLuint  PyramidVBO[2], PyramidEBO;
	glGenBuffers(2, PyramidVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &PyramidEBO);
	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Third_Rect), Third_Rect, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PyramidEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Third_Rect_Index), Third_Rect_Index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Third_Rect_Colors), Third_Rect_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}
void InitBuffer4()
{
	glGenVertexArrays(1, &Ground_RectVAO);
	GLuint  PyramidVBO[2], PyramidEBO;
	glGenBuffers(2, PyramidVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &PyramidEBO);
	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Ground_Rect), Ground_Rect, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PyramidEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Ground_Index), Ground_Index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Ground_Colors), Ground_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}