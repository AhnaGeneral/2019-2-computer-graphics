#include "source.h"

const int TIMER_TICK = 10;

void drawScene();
void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void InitBuffer();
void TimerFunc(int value);
void MenuFunc(int button);
unsigned int ShapeVAO, PyramidVAO;
float Angle = 90.0f;
GLUquadricObj * object;
GLUquadricObj * objDisk;
bool boolperspective = true, boolortho = false;
int modelnumber = 1;
bool PolyModeFILL = true;
float posx = 0.0f, posy = 0.0f, posz = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 8.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 0.5f, 0.0f);

GLfloat Axes[] =
{ /*X축*/ +1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f,
/*Y축*/  +0.0f, +1.0f, +0.0f, +0.0f, -1.0f, +0.0f,
/*Z축*/  +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, -1.0f };

unsigned int index[] = { 0, 1 , 2, 3 , 4, 5 };

GLfloat Colors[] =
{ /*X축*/ +1.0f, +0.0f, +0.0f,  1.0f, +0.0f, +0.0f,
/*Y축*/   +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f,
/*Z축*/  +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f };

float cameraRotate = 0.0f;
float sunRotate = 0.0f;
float earthRotate = 0.0f;

GLuint pid;


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

	pid = (Complie_Shaders());


	//메뉴만들기
	int submenu;
	submenu = glutCreateMenu(MenuFunc);
	glutAddMenuEntry("FILL", 1);
	glutAddMenuEntry("LINE", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	object = gluNewQuadric(); // 객체생성하기
	objDisk = gluNewQuadric();

	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	//glutMouseFunc(MouseEvent);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutTimerFunc(TIMER_TICK, TimerFunc, 1);
	glutMainLoop();
}

void MenuFunc(int button) {
	// button: 메뉴의구별자값 
	switch (button)
	{
	case 1:
		PolyModeFILL = false;
		break;
	case 2:
		PolyModeFILL = true;
		break;
	}
	glutPostRedisplay();
}


void drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(pid);

	int modelLoc = glGetUniformLocation(pid, "model");
	int viewLoc = glGetUniformLocation(pid, "view");
	int projLoc = glGetUniformLocation(pid, "projection");

	glBindVertexArray(ShapeVAO); // VAO를 바인드하기

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	view = glm::translate(view, glm::vec3(posx, posy, posz));
	view = glm::rotate(view, glm::radians(cameraRotate), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	if (boolperspective)
	{
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
	}
	if (boolortho)
	{
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(float(1.0), float(1.0), float(1.0), float(1.0), 0.1f, 200.0f);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
	}//projection = glm::ortho((float)WINDOW_SIZE_WIDTH * -0.5, (float)WINDOWe_SIZE_WIDTH * 0.5, (float)WINDOW_SIZE_HEIGHT * -0.5, (float)WINDOW_SIZE_HEIGHT * 0.5);
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 scaleMatrix = glm::mat4(1.0f); // 단위행렬로초기화 
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(3.0, 3.0, 3.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(scaleMatrix));

	InitBuffer();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//if (PolyModeFILL)
	//gluQuadricDrawStyle(object, GLU_POINT);
	//else 
	gluQuadricDrawStyle(object, GLU_FILL); // 도형스타일 

	for (auto degree : { 0.0f, 45.0f, 135.0f })
	{
		auto sunMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));
		sunMatrix = glm::rotate(sunMatrix, glm::radians(sunRotate), glm::vec3(0.0f, 1.0f, 0.0f));
		sunMatrix = glm::rotate(sunMatrix, glm::radians(Angle), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sunMatrix));
		if (!degree)
		{
			gluSphere(object, 0.7f, 30, 30); //태양을 한번만 그려라.
		}
		gluDisk(objDisk, 1.95, 2.0, 20, 1);

		auto earthmatrix = glm::rotate(sunMatrix, glm::radians(-Angle), glm::vec3(1.0f, 0.0f, 0.0f));
		earthmatrix = glm::translate(earthmatrix, glm::vec3(-2.0f, 0.0f, 0.0f));
		earthmatrix = glm::rotate(earthmatrix, glm::radians(earthRotate + (degree / 45.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		earthmatrix = glm::rotate(earthmatrix, glm::radians(Angle), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(earthmatrix));
		gluSphere(object, 0.3f, 30, 30);
		gluDisk(objDisk, 0.5, 0.52, 20, 1);

		auto moonMatrix = glm::rotate(earthmatrix, glm::radians(-Angle), glm::vec3(1.0f, 0.0f, 0.0f));
		moonMatrix = glm::translate(earthmatrix, glm::vec3(-0.5f, 0.0f, 0.0f));
		moonMatrix = glm::rotate(moonMatrix, glm::radians(Angle), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(moonMatrix));
		gluSphere(object, 0.1f, 30, 30);
		//gluDisk(objDisk, 1.95, 2.0, 20, 1);
	}

	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'w') posy -= 0.2;
	if (key == 's') posy += 0.2;
	if (key == 'd') posx -= 0.2;
	if (key == 'a') posx += 0.2;

	if (key == 'z') posz += 0.2;
	if (key == 'x') posz -= 0.2;

	if (key == 'y') cameraRotate += 10.0;
	if (key == 'Y') cameraRotate -= 10.0;


	if (key == 'p')
	{
		boolperspective = true;
		boolortho = false;
	}
	if (key == 'P')
	{
		boolperspective = false;
		boolortho = true;
	}

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

void TimerFunc(int value)
{
	sunRotate += 0.5;
	earthRotate += 5.0;

	glutPostRedisplay();
	glutTimerFunc(TIMER_TICK, TimerFunc, 1);
}