#include "source.h"

void drawScene();
void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void InitBuffer();
void InitBuffer2();
void InitBuffer3();
unsigned int ShapeVAO, PyramidVAO;
float LeftAngle = 0.0f, RightAngle = 0.0f;
float LeftAngleS = 0.0f, RightAngleS = 0.0f;
GLUquadricObj *model1;
int modelnumber = 1;
bool PolyModeFILL = false;
float posx =0.0f, posy=0.0f, posz=0.0f;
bool Tri = true, Pyra = false;
bool Revolve =true;
void DefaultTimerFunction(int);

                          
GLfloat TriShape[] =
{
-0.5f, -0.5f, -0.5f,  -0.5f, -0.5f, +0.5f, -0.5f, +0.5f, -0.5f,
-0.5f, +0.5f, +0.5f, +0.5f, -0.5f, -0.5f,  +0.5f, -0.5f, +0.5f,
+0.5f, +0.5f, -0.5f,  +0.5f, +0.5f, +0.5f
};

GLfloat TriColors[] = 
{  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 
   0.0f, 0.0f, 1.0f,  1.0f , 1.0f ,0.0f,
   1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,
   0.2f, 0.2f, 1.0f,  0.0f , 0.5f ,1.0f };

unsigned int Triindex[] =
{ 2,6,4, 0,2,4, 5,6,7,5,4,6,2,7,6,2,3,7,3,2,1,2,0,1,5,7,3,5,3,1,0,4,5,0,5,1 };

GLfloat Pyramid[] =
{
  +0.0f, +1.0f, +0.0f, +0.5f, +0.0f, -0.5f, +0.5f, +0.0f, +0.5f,
  -0.5f, +0.0f, +0.5f, -0.5f, +0.0f, -0.5f
};

unsigned int Pyramid_Index[] = { 1,3,4, 1,2,3, 0,4,1, 0,1,2, 0,3,4, 0,2,3 };

GLfloat Axes[] = { /*Y축*/  +0.0f, +1.0f, +0.0f, +0.0f, -1.0f, +0.0f };

unsigned int index[] = { 0, 1 };

GLfloat Axes2[] =
{ /*X축*/ +1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f
/*Z축*/ //+0.0f, +0.0f, +1.0f, +0.0f, +0.0f, -1.0f
};

unsigned int index2[] = { 0, 1 , 2, 3 };

GLfloat Colors[] =
{ /*X축*/ +1.0f, +0.0f, +0.0f,  1.0f, +0.0f, +0.0f,
/*Y축*/   +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f,
/*Z축*/  +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f };

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
	glutSpecialFunc(SpecialKeyboard); 
	//glutMouseFunc(MouseEvent);
	glutTimerFunc(1, DefaultTimerFunction, 0);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutMainLoop();
}

void drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(Complie_Shaders());

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 8.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.5f, 0.0f);

	int modelLoc = glGetUniformLocation(Complie_Shaders(), "model");
	int viewLoc = glGetUniformLocation(Complie_Shaders(), "view");
	int projLoc = glGetUniformLocation(Complie_Shaders(), "projection");
	
	//[ 좌표축 만들기 ]/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(ShapeVAO); // VAO를 바인드하기
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
 
	glm::mat4 Matrix = glm::mat4(1.0f); 
	Matrix = glm::scale(Matrix, glm::vec3(3.0, 3.0, 3.0));
	//Matrix = glm::rotate(Matrix, glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Matrix));
	InitBuffer(); 

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	//[ 첫번째 도형 만들기 ]//////////////////////////////////////////////////////////////////////////////////////////////
	model1 = gluNewQuadric(); // 객체생성하기
	if(PolyModeFILL) gluQuadricDrawStyle(model1, GLU_FILL);
	else gluQuadricDrawStyle(model1, GLU_LINE); // 도형스타일 
	glm::mat4 model1trans = glm::mat4(1.0f);

	// 객체만들기
	if (modelnumber==1) 
	{      // 자전
		if (Revolve == false)
		{
			model1trans = glm::mat4(1.0f);
			model1trans = glm::translate(model1trans, glm::vec3(-1.5f, 0.0f, 0.0f));
			model1trans = glm::rotate(model1trans, glm::radians(LeftAngle), glm::vec3(0.0f, 1.0, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1trans));
			
			if (Tri == true && Pyra == false)
				gluSphere(model1, 1.0f, 30, 30);
			if (Tri == false && Pyra == true)
				gluCylinder(model1, 1.0, 0.0, 2.0, 30, 8);
		} 

		if (Revolve == true)
		{
			model1trans = glm::mat4(1.0f);
			model1trans = glm::rotate(model1trans, glm::radians(LeftAngleS), glm::vec3(0.0f, 1.0, 0.0f));
			model1trans = glm::translate(model1trans, glm::vec3(-1.5f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1trans));
			if (Tri == true && Pyra == false)
				gluSphere(model1, 1.0f, 30, 30);
			if (Tri == false && Pyra == true)
				gluCylinder(model1, 1.0, 0.0, 2.0, 30, 8);
		}
	}

	//[ 두번째 도형 만들기 ]/////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(PyramidVAO); 
	if (Revolve == false)
	{
		model1trans = glm::mat4(1.0f);
		model1trans = glm::scale(model1trans, glm::vec3(3.0, 3.0, 3.0));
		//model1trans = glm::rotate(model1trans, glm::radians(RightAngleS), glm::vec3(0.0f, 1.0f, 0.0f));
		model1trans = glm::translate(model1trans, glm::vec3(0.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1trans));
		InitBuffer3();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);

		model1trans = glm::mat4(1.0f);
		model1trans = glm::translate(model1trans, glm::vec3(1.5f, 0.0f, 0.0f));
		model1trans = glm::rotate(model1trans, glm::radians(RightAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1trans));
	}

	if (Revolve == true)
	{
		model1trans = glm::mat4(1.0f);
		model1trans = glm::rotate(model1trans, glm::radians(RightAngleS), glm::vec3(0.0f, 1.0f, 0.0f));
		model1trans = glm::translate(model1trans, glm::vec3(1.5f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1trans));
	}
	InitBuffer2();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(ShapeVAO);
	if (Revolve == true)
	{
		model1trans = glm::mat4(1.0f);
		model1trans = glm::scale(model1trans, glm::vec3(3.0, 3.0, 3.0));
		model1trans = glm::rotate(model1trans, glm::radians(RightAngleS), glm::vec3(0.0f, 1.0f, 0.0f));
		model1trans = glm::translate(model1trans, glm::vec3(0.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1trans));
	    InitBuffer3();
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 	    glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);
	}

	// VAO를 바인드하기
	//model2 = gluNewQuadric(); // 객체생성하기
	//if (PolyModeFILL) gluQuadricDrawStyle(model2, GLU_FILL);
	//else gluQuadricDrawStyle(model2, GLU_LINE); // 도형스타일 
	//
	////glm::mat4 model1trans = glm::mat4(1.0f);
	//// 객체만들기
	//if (modelnumber == 1)
	//{
	//	if (Revolve == false)
	//	{
	//		model1trans = glm::mat4(1.0f);
	//		model1trans = glm::translate(model1trans, glm::vec3(posx, posy, posz));
	//		model1trans = glm::rotate(model1trans, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
	//		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1trans));
	//	}
	//	if (Revolve == true)
	//	{
	//		model1trans = glm::mat4(1.0f);
	//		model1trans = glm::rotate(model1trans, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
	//		model1trans = glm::translate(model1trans, glm::vec3(posx, posy, posz));
	//		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1trans));
	//	}
	//	gluDisk(model2, 0.5f, 2.0f, 20,3);
	//}
	glutSwapBuffers();
}

int ModeCounter = 0;
int ModeRotate = 0;
int changecounter = 0;
void Keyboard(unsigned char key, int x, int y)
{
	if (key == '1') { modelnumber = 1; };
	if (key == '2') { modelnumber = 2; };
	if (key == '3') { modelnumber = 3; };
	if (key == '4') { modelnumber = 4; };
	if (key == '5') { modelnumber = 5; };

	if (key == 'w' || key == 'W')
	{
		if (ModeCounter == 2)
		{
			ModeCounter = 0;
		}
		if (ModeCounter)
			PolyModeFILL = true;
		else PolyModeFILL = false;
		++ModeCounter;
	}
	if (key == 'y') { LeftAngle += 15.0f; Revolve = false; }
	if (key == 'Y') { LeftAngle -= 15.0f; Revolve = false;	}

	if (key == 'x') { RightAngle += 15.0f; Revolve = false;}
	if (key == 'X') { RightAngle -= 15.0f; Revolve = false;}

	if (key == 'b') 
	{ 
		Revolve = true;
		RightAngleS += 15.0f; 
		LeftAngleS += 15.0f; 
	}
	if (key == 'B') 
	{
		Revolve = true;
		RightAngleS -= 15.0f;
		LeftAngleS -= 15.0f;
	}

	if (key == 'c')
	{
		if (changecounter == 2)
		{
			changecounter = 0;
		}
		if (changecounter) 
		{
			Tri = true, Pyra = false;
		}
		else
		{
			Tri = false, Pyra = true;
		}
		++changecounter;
	
	}
	
	glutPostRedisplay();
}

void DefaultTimerFunction(int value)
{
	glutPostRedisplay(); // 화면재출력
	glutTimerFunc(1, DefaultTimerFunction, 0); // 타이머함수재설정
}

void SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)  posy += 0.2;
	if (key == GLUT_KEY_DOWN) posy -= 0.2;
	if (key == GLUT_KEY_RIGHT) posx += 0.2;
	if (key == GLUT_KEY_LEFT) posx -= 0.2;
	glutPostRedisplay();
}


void InitBuffer()
{
	glGenVertexArrays(1, &ShapeVAO);
	GLuint  ShapeVBO[2], ShapeEBO;
	glGenBuffers(2, ShapeVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &ShapeEBO);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Axes), Axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ShapeEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

}
void InitBuffer3()
{
	glGenVertexArrays(1, &ShapeVAO);
	GLuint  ShapeVBO[2], ShapeEBO;
	glGenBuffers(2, ShapeVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &ShapeEBO);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Axes2), Axes2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ShapeEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index2), index2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

}
void InitBuffer2()
{
	glGenVertexArrays(1, &PyramidVAO);
	GLuint  ShapeVBO[2], ShapeEBO;
	glGenBuffers(2, ShapeVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &ShapeEBO);
	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[0]);

	if (Tri == true && Pyra == false)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriShape), TriShape, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ShapeEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Triindex), Triindex, GL_STATIC_DRAW);
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