#include "source.h"

void drawScene();
void InitBuffer();
GLuint VAO[4];
GLuint ProgramID;
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
GLvoid Reshape(int w, int h);
GLuint LineVBO[2], LineEBO;
GLuint TriVBO[2], TriEBO;
GLuint RectVBO[2], RectEBO;

GLfloat Axes[6] { 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
unsigned int Axes_Index[] = { 0, 1 };
GLfloat Colors[] = { 1.0f, 0.0f, 0.0f,  1.0f, 0.8f, 0.8f };
float glx1, gly1, glx2, gly2; 

//float Center[3]{ 0.0f,0.0f,0.0f };

class Points
{
public:
	float x; 
	float y; 
	float z; 
	Points() :x{ 0.0f }, y{ 0.0f }, z{ 0.0f }
	{
	}
};

Points Center; 

GLfloat TriShape[]
{
Center.x, Center.y + 0.1f, 0.0f,
Center.x - 0.1f, Center.y - 0.1f, 0.0f,
Center.x + 0.1f, Center.y - 0.1f, 0.0f
};

GLfloat RectShape[]
{
Center.x + 0.1f, Center.y + 0.1f, 0.0f,
Center.x - 0.1f, Center.y + 0.1f, 0.0f,
Center.x + 0.1f, Center.y - 0.1f, 0.0f ,
Center.x - 0.1f, Center.y - 0.1f, 0.0f ,
};

void main(int argc, char** argv)
{
	
	glutInit(&argc, argv); //glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //디스플레이 모드 설정
	glutInitWindowPosition(100, 100); //윈도우의 위치 지정
	glutInitWindowSize(g_window_w, g_window_h); //윈도우의 크기 지정
	glutCreateWindow("using index buffer");//윈도우 생성

	//[ GLEW 초기화 하기 ]=======================================================
	glewExperimental = GL_TRUE; //glew 초기화
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized\n";
	//==========================================================================

	ProgramID = (Complie_Shaders());

	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void Mouse(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      //std::cout << "x = " << x << " y = " << y << std::endl;
      convertDeviceXYOpenGLXY(x, y, &glx1, &gly1);
      //std::cout << "glx = " << glx1 << " gly = " << gly1 << std::endl;
      Axes[0] = glx1; 
      Axes[1] = gly1;
   }
   glutPostRedisplay();
}

void Motion(int x, int y)
{
   //std::cout << "motionx = " << x << "motiony = " << y << std::endl;
   convertDeviceXYOpenGLXY(x, y, &glx2, &gly2);
   //std::cout << "glx = " << glx2 << " gly = " << gly2 << std::endl;
   Axes[3] = glx2;
   Axes[4] = gly2;
   glutPostRedisplay();
}

void drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ProgramID);

	glBindVertexArray(VAO[0]);
	InitBuffer();
	glDrawArrays(GL_LINES, 0, 2);
	//glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	glutSwapBuffers();
	//glutPostRedisplay();
}

void InitBuffer()
{
	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(4, VAO);
	glGenBuffers(2, LineVBO);

	// first triangle VAO VBO setting
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, LineVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Axes), Axes, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);


	// line VAO VBO setting
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, LineVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Axes), Axes, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//glGenVertexArrays(4, VAO);
	////GLuint LineVBO[2], LineEBO;
	////GLuint TriVBO[2], TriEBO; 
	////GLuint RectVBO[2], RectEBO; 
	//glGenBuffers(2, LineVBO);                  // 2개의 VBO지정하고 할당하기
	//glGenBuffers(1, &LineEBO);

	//glBindVertexArray(VAO[0]);
	//glBindBuffer(GL_ARRAY_BUFFER, LineVBO[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Axes), Axes, GL_STATIC_DRAW);
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LineEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Axes_Index), Axes_Index, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//glEnableVertexAttribArray(0);

	////glBindBuffer(GL_ARRAY_BUFFER, LineVBO[1]);
	////glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	////glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	////glEnableVertexAttribArray(1);

	//glBindVertexArray(VAO[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, TriVBO[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(TriVBO), TriVBO, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TriEBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Axes_Index), Axes_Index, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, TriVBO[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(TriVBO), TriVBO, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);
}

