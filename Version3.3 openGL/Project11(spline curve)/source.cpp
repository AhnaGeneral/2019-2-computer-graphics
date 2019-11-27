#include "source.h"
#include <math.h>

#define SIN_CNT 360
#define ZIGZAG_CNT 600
#define SPRING_CNT 650

unsigned int SinVAO, ZigZagVAO, SpringVAO;
GLuint ProgramID;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat SinShape[SIN_CNT][3] = {0};
GLfloat ZigZagShape[ZIGZAG_CNT][3] = { 0 };
GLfloat SpringShape[SPRING_CNT][3] = { 0 };
bool SinAble = true, ZigZagable = false, Springable = false;
//==========================================================================
void Keyboard(unsigned char key, int x, int y); // x, y 키보드를 입력 할 당시 마우스 위치
//void SpecialKeyboard(int key, int x, int y);
//==========================================================================

void SinBuffer();
void ZigZagBuffer();
void SpringBuffer();

void drawScene();

void SinLine_Ready_Object();
void ZigZagLine_Ready_Object();
void Spring_Ready_Object();


// [ sinLine Fill ] =========================================================
void SinLine_Ready_Object()
{
	GLfloat fAngle = -360.f; 
	
	for (int i = 0; i < SIN_CNT; ++i)
	{
		fAngle += 2.0f; 
		
		SinShape[i][0] = fAngle / 360.f;  // -1 ~ 1 
		SinShape[i][1] = sinf(glm::radians(fAngle))*0.7f; 
		SinShape[i][2] = 0.0f; 
	}
}
// [ zigzag Fill ] ==========================================================
void ZigZagLine_Ready_Object()
{
	GLfloat fAngle = -360.f;
	GLfloat Positionx = -1.5f; 

	for (int i = 0; i < ZIGZAG_CNT; ++i)
	{
		fAngle += 2.0f;
		Positionx += 0.003f; 

		ZigZagShape[i][0] = Positionx;
		ZigZagShape[i][1] = sinf(glm::radians(fAngle))*0.7f;
		ZigZagShape[i][2] = 0.0f;
	}
}

// [ Spring Fill ] =========================================================
void Spring_Ready_Object()
{
	GLfloat fAngle = -360.f;
	GLfloat Positionx = -1.5f;

	for (int i = 0; i < SPRING_CNT; ++i)
	{
		fAngle += 2.0f;
		Positionx += 0.005f;
		SpringShape[i][0] = cosf(glm::radians(fAngle))*0.5f + Positionx ;
		SpringShape[i][1] = sinf(glm::radians(fAngle))*0.7f;
		SpringShape[i][2] = 0.0f;
	}
}
// ==========================================================================

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
	ProgramID= (Complie_Shaders());
	
	SinLine_Ready_Object();
	ZigZagLine_Ready_Object();
	Spring_Ready_Object(); 
	
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	//glutMouseFunc(MouseEvent);
	//glutReshapeFunc (); // 다시 그리기 함수 지정 
	glutMainLoop();
}

void drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glDisable(GL_CULL_FACE);
	glUseProgram(ProgramID);

	int modelLoc = glGetUniformLocation(ProgramID, "model");
	int viewLoc = glGetUniformLocation(ProgramID, "view");
	int projLoc = glGetUniformLocation(ProgramID, "projection");

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 scaleMatrix = glm::mat4(1.0f); // 단위행렬로초기화 
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(1.0, 1.0, 1.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(scaleMatrix));

	if (SinAble == true) 
	{
		glBindVertexArray(SinVAO);
		SinBuffer();
		glDrawArrays(GL_LINE_STRIP, 0, SIN_CNT);
	}

	if (ZigZagable == true)
	{
		glBindVertexArray(ZigZagVAO);
		ZigZagBuffer();
		glDrawArrays(GL_LINE_STRIP, 0, ZIGZAG_CNT);
	}

	if (Springable == true)
	{
		glBindVertexArray(SpringVAO);
		SpringBuffer();
		glDrawArrays(GL_LINE_STRIP, 0, SPRING_CNT);
	}

	glutSwapBuffers();
}

void SinBuffer()
{
	glGenVertexArrays(1, &SinVAO);
	GLuint  VBO;
	glGenBuffers(1, &VBO);   // 2개의 VBO지정하고 할당하기

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SinShape), SinShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}

void ZigZagBuffer()
{
	glGenVertexArrays(1, &ZigZagVAO);
	GLuint  VBO;
	glGenBuffers(1, &VBO);   // 2개의 VBO지정하고 할당하기

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ZigZagShape), ZigZagShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}

void SpringBuffer()
{
	glGenVertexArrays(1, &SpringVAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);   // 2개의 VBO지정하고 할당하기

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpringShape), SpringShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
}


void Keyboard(unsigned char key, int x, int y)
{
	if (key == '1') { SinAble = true; ZigZagable = false; Springable = false; }
	if (key == '2') { SinAble = false; ZigZagable = true; Springable = false; }
	if (key == '3') { SinAble = false; ZigZagable = false; Springable = true; }
	glutPostRedisplay();
	
}