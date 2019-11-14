#include "source.h"

void drawScene();
void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void InitBuffer();
GLvoid Reshape(int w, int h);

unsigned int ShapeVAO, PyramidVAO;

float Angle = 90.0f;
GLUquadricObj *Model; 
int modelnumber = 1;
bool PolyModeFILL = false;
float posx =0.0f, posy=0.0f, posz=0.0f;

bool Revolve =false;
                          
GLfloat Axes[] =
{ /*X축*/ +1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f,
  /*Y축*/  +0.0f, +1.0f, +0.0f, +0.0f, -1.0f, +0.0f,
  /*Z축*/  +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, -1.0f };

unsigned int index[] = { 0, 1 , 2, 3 , 4, 5 };

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
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutReshapeFunc(Reshape);
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
	
	glBindVertexArray(ShapeVAO); // VAO를 바인드하기
	
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 scaleMatrix = glm::mat4(1.0f); // 단위행렬로초기화 
	scaleMatrix = glm::scale (scaleMatrix, glm::vec3 (3.0, 3.0, 3.0)); 
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(scaleMatrix));


	InitBuffer(); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

	view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx, posy, posz));



	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();

	Model = gluNewQuadric(); // 객체생성하기
	if(PolyModeFILL) gluQuadricDrawStyle(Model, GLU_FILL); 
	else gluQuadricDrawStyle(Model, GLU_LINE); // 도형스타일 

	// 객체만들기
	if (modelnumber==1) 
	{ 
		if (Revolve == false)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(posx, posy, posz));
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		if (Revolve == true)
		{
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			model = glm::translate(model, glm::vec3(posx, posy, posz));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		gluSphere(Model, 1.0f, 30, 30);
	}
	if (modelnumber==2) 
	{ 
		if (Revolve == false)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(posx, posy, posz));
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(1.0f, 0.0, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		if (Revolve == true)
		{
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			model = glm::translate(model, glm::vec3(posx, posy, posz));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		gluCylinder(Model, 1.0, 1.0, 2.0, 30, 8);
	}
	if (modelnumber==3) {
		if (Revolve == false)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(posx, posy, posz));
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		if (Revolve == true)
		{
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			model = glm::translate(model, glm::vec3(posx, posy, posz));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		gluDisk(Model, 0.5, 2.0, 30, 3); 
	}
	if (modelnumber==4) 
	{
		if (Revolve == false)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(posx, posy, posz));
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(1.0f, 0.0, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		if (Revolve == true)
		{
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(1.0f, 0.0, 0.0f));
			model = glm::translate(model, glm::vec3(posx, posy, posz));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		gluCylinder(Model, 1.0, 0.0, 2.0, 30, 8);}
	if (modelnumber==5) { 
		if (Revolve == false)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(posx, posy, posz));
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		if (Revolve == true)
		{
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(Angle), glm::vec3(0.0f, 1.0, 0.0f));
			model = glm::translate(model, glm::vec3(posx, posy, posz));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}
		gluCylinder(Model, 1.0, 0.0, 2.0, 30, 8);}

	glutSwapBuffers();

}
int ModeCounter = 0;
int ModeRotate = 0;
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
	if (key == 'y') { Angle += 15.0f; Revolve = false; }
	if (key == 'Y') { Angle -= 15.0f; Revolve = false; }

	if (key == 'h') { Angle += 15.0f; Revolve = true; }
	if (key == 'H') { Angle -= 15.0f; Revolve = true; }

	glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)  posy += 0.2;
	if (key == GLUT_KEY_DOWN) posy -= 0.2;
	if (key == GLUT_KEY_RIGHT) posx += 0.2;
	if (key == GLUT_KEY_LEFT) posx -= 0.2;
	glutPostRedisplay();
}

GLvoid Reshape(int w, int h) // 콜백 함수: 다시 그리기 
{
	glViewport(0, 0, w, h);
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