#include "source.h"
#include <math.h>
#define CNT_BEZIER 1000

unsigned int VAO_Bezier, VAO_Rect, VAO_Tri, VAO_Axes;
GLuint ProgramID;
//==========================================================================
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat BezierCurve[CNT_BEZIER][3] = { 0 };
GLfloat ControlPoint[5][3] = {0}; 
float CameraX = 0.0f , CameraY = 0.0f, CameraZ = 0.0f;
float Speed = 1.0f;
bool StartMove = false; 
float glx1, gly1, glx2, gly2;
bool modeFill = false, modeLine = true, RouteON = true;
int Random; 
//==========================================================================
int KeyCount = 0;
void Keyboard(unsigned char key, int x, int y); // x, y 키보드를 입력 할 당시 마우스 위치
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Timerfunction(int value);
//void SpecialKeyboard(int key, int x, int y);
//==========================================================================
void drawScene();
void BezierBuffer();
void RectBuffer();
void TriBuffer();
void AxesBuffer();
void BezierCurve_Ready_Object();
void FillArrayTri();
void FillArrayRect();

// ==========================================================================
GLfloat TriPos[3] = { 0.0f, 0.0f, 0.0f };
GLfloat RectPos[3] = { 0.0f, 0.0f, 0.0f };

GLfloat TriShape[3][3]
{ {TriPos[0]-0.1f, TriPos[1]-0.1f, +0.0f},
  {TriPos[0]+0.1f, TriPos[1]-0.1f, +0.0f}, 
  {TriPos[0]+0.0f, TriPos[1]+0.1f, +0.0f} };

GLfloat RectShape[4][3]
{ {RectPos[0]-0.1f, RectPos[1]-0.1f, 0.0f},
  {RectPos[0]-0.1f, RectPos[1]+0.1f, 0.0f},
  {RectPos[0]+0.1f, RectPos[1]+0.1f, 0.0f},
  {RectPos[0]+0.1f, RectPos[1]-0.1f, 0.0f} };

GLfloat Axes[6]{ 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

unsigned int TriIndex[] = { 0, 1, 2 };
unsigned int RectIndex[] = { 0,1,3,2,1,3 };
unsigned int Axes_Index[] = { 0, 1 };

// ==========================================================================

void main(int argc, char** argv)
{
	glutInit(&argc, argv); //glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //디스플레이 모드 설정
	glutInitWindowPosition(100, 100); //윈도우의 위치 지정
	glutInitWindowSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT); //윈도우의 크기 지정
	glutCreateWindow("using index buffer");//윈도우 생성

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	//[ GLEW 초기화 하기 ]=====================================================
	glewExperimental = GL_TRUE; //glew 초기화
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized\n";
	ProgramID = (Complie_Shaders());
	Random = rand() % 4;

	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
    glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glutTimerFunc(1000,Timerfunction,1);
	//glutReshapeFunc (); // 다시 그리기 함수 지정 
	glutMainLoop();
}


int arr = 0;

void FillArrayTri()
{
	TriShape[0][0] = TriPos[0] - 0.1f;
	TriShape[0][1] = TriPos[1] - 0.1f;
	TriShape[0][2] = 0.0f;

	TriShape[1][0] = TriPos[0] + 0.1f;
	TriShape[1][1] = TriPos[1] - 0.1f;
	TriShape[1][2] = 0.0f;

	TriShape[2][0] = TriPos[0] + 0.0f;
	TriShape[2][1] = TriPos[1] + 0.1f;
	TriShape[2][2] = 0.0f;
}
void FillArrayRect()
{
	RectShape[0][0] = RectPos[0] - 0.1f;
	RectShape[0][1] = RectPos[1] - 0.1f;
	RectShape[0][2] = 0.0f;

	RectShape[1][0] = RectPos[0] - 0.1f;
	RectShape[1][1] = RectPos[1] + 0.1f;
	RectShape[1][2] = 0.0f;

	RectShape[2][0] = RectPos[0] + 0.1f;
	RectShape[2][1] = RectPos[1] + 0.1f;
	RectShape[2][2] = 0.0f;

	RectShape[3][0] = RectPos[0] + 0.1f;
	RectShape[3][1] = RectPos[1] - 0.1f;
	RectShape[3][2] = 0.0f;
}
void drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ProgramID);

	int modelLoc = glGetUniformLocation(ProgramID, "model");

	glm::mat4 model;
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//======================================================================================


	if (KeyCount >= 5)
	{
		BezierCurve_Ready_Object();
		glBindVertexArray(VAO_Bezier);
		BezierBuffer();
		glLineWidth(1.5f);
		if (RouteON) { glDrawArrays(GL_LINE_STRIP, 0, CNT_BEZIER); }
		StartMove = true;
	}

	if (arr >= 950)
	{
		arr = 0; 
		Random = rand() % 4;

	}
	if (StartMove == true)
	{
		if (modeFill)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (modeLine)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//=====================================================
		if (Random == 0)
		{
			TriPos[0] = BezierCurve[arr][0];
			TriPos[1] = BezierCurve[arr][1];
			FillArrayTri();
			glBindVertexArray(VAO_Tri);
			TriBuffer();
			glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
		}
		if (Random == 1)
		{
			TriPos[0] = BezierCurve[CNT_BEZIER - arr][0];
			TriPos[1] = BezierCurve[CNT_BEZIER - arr][1];
			FillArrayTri();
			glBindVertexArray(VAO_Tri);
			TriBuffer();
			glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
		}
		//=====================================================
		if (Random == 2)
		{
			RectPos[0] = BezierCurve[CNT_BEZIER - arr][0];
			RectPos[1] = BezierCurve[CNT_BEZIER - arr][1];
			FillArrayRect();
			glBindVertexArray(VAO_Rect);
			RectBuffer();
			glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
		}
		if (Random == 3)
		{
			RectPos[0] = BezierCurve[arr][0];
			RectPos[1] = BezierCurve[arr][1];
			FillArrayRect();
			glBindVertexArray(VAO_Rect);
			RectBuffer();
			glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
		}
		//=====================================================

		glBindVertexArray(VAO_Axes);
		AxesBuffer();
		glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_INT, 0);
	}

	glutSwapBuffers();
}

void BezierBuffer()
{
	glGenVertexArrays(1, &VAO_Bezier);
	GLuint  VBO;
	glBindVertexArray(VAO_Bezier);
	glGenBuffers(1, &VBO);   // 2개의 VBO지정하고 할당하기
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BezierCurve), BezierCurve, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}

void RectBuffer()
{
	glGenVertexArrays(1, &VAO_Rect);
	GLuint  VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO_Rect);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RectShape), RectShape, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RectIndex), RectIndex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}

void TriBuffer()
{
	glGenVertexArrays(1, &VAO_Tri);
	GLuint  VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO_Tri);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriShape), TriShape, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TriIndex), TriIndex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}

void AxesBuffer()
{
	glGenVertexArrays(1, &VAO_Axes);
	GLuint  VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO_Axes);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Axes), Axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Axes_Index), Axes_Index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}


void BezierCurve_Ready_Object()
{
	float fInterT = 0.0f;        // (1 - fT)
	float fT = 0.0f;
	for (int i = 0; i < 1000; ++i)
	{
		fInterT = (1.0f - fT);

		BezierCurve[i][0] = ControlPoint[0][0] * powf(fInterT, 4.0f) +
			ControlPoint[1][0] * 4.0f * fT * powf(fInterT, 3.0f) +
			ControlPoint[2][0] * 6.0f * powf(fT, 2.0f) * powf(fInterT, 2.0f) +
			ControlPoint[3][0] * 4.0f * powf(fT, 3.0f) * fInterT +
			ControlPoint[4][0] * powf(fT, 4.0f);

		BezierCurve[i][1] = ControlPoint[0][1] * powf(fInterT, 4.0f) +
			ControlPoint[1][1] * 4.0f * fT * powf(fInterT, 3.0f) +
			ControlPoint[2][1] * 6.0f * powf(fT, 2.0f) * powf(fInterT, 2.0f) +
			ControlPoint[3][1] * 4.0f * powf(fT, 3.0f) * fInterT +
			ControlPoint[4][1] * powf(fT, 4.0f);

		BezierCurve[i][2] = 0.0f;

		fT += 0.001f;
	}
}


void Timerfunction(int value)
{
	if (StartMove == true)
	{
		arr += Speed;
	}
	glutPostRedisplay();
	glutTimerFunc(100, Timerfunction, 1);

}


void Motion(int x, int y)
{
	convertDeviceXYOpenGLXY(x, y, &glx2, &gly2);
	Axes[3] = glx2;
	Axes[4] = gly2;
}

float glx = 0.0f, gly = 0.0f;
void Mouse(int button, int state, int x, int y)
{
	if (KeyCount < 6)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			std::cout << "x = " << x << " y = " << y << std::endl;
			convertDeviceXYOpenGLXY(x, y, &glx, &gly);
			ControlPoint[KeyCount][0] = glx;
			ControlPoint[KeyCount][1] = gly;
			ControlPoint[KeyCount][2] = 0.0f;
			++KeyCount;
		}
	}
	
	if (KeyCount >= 6)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			convertDeviceXYOpenGLXY(x, y, &glx1, &gly2);
			Axes[0] = glx1;
			Axes[1] = gly2;
		}
	}
	
	glutPostRedisplay();
	
}


void Keyboard(unsigned char key, int x, int y)
{
	//========================================
	if (key == 'w') CameraY -= 0.2;
	if (key == 's') CameraY += 0.2;
	if (key == 'd') CameraX -= 0.2;
	if (key == 'a') CameraX += 0.2;
	if (key == 'z') CameraZ += 0.2;
	if (key == 'x') CameraZ -= 0.2;
	//========================================
	if (key == '1') Speed += 1.0f;
	if (key == '2') Speed -= 1.0f;

	if (key == 'q') exit(0); 
	if (key == 'c') KeyCount = 0;
	if (key == 'f')
	{
		modeFill = false;
		modeLine = true;
	}
	
	if (key == 'F')
	{
		modeFill = true;
		modeLine = false;
	}
	if (key == 'v') RouteON = false;
	if (key == 'V') RouteON = true;

	glutPostRedisplay();
}