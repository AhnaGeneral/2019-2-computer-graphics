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
	
	glutInit(&argc, argv); //glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //���÷��� ��� ����
	glutInitWindowPosition(100, 100); //�������� ��ġ ����
	glutInitWindowSize(g_window_w, g_window_h); //�������� ũ�� ����
	glutCreateWindow("using index buffer");//������ ����

	//[ GLEW �ʱ�ȭ �ϱ� ]=======================================================
	glewExperimental = GL_TRUE; //glew �ʱ�ȭ
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
	glutDisplayFunc(drawScene); // ��� �Լ��� ���� 
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
	//// 5.1. VAO ��ü ���� �� ���ε�
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
	//glGenBuffers(2, LineVBO);                  // 2���� VBO�����ϰ� �Ҵ��ϱ�
	//glGenBuffers(1, &LineEBO);

	//glBindVertexArray(VAO[0]);
	//glBindBuffer(GL_ARRAY_BUFFER, LineVBO[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Axes), Axes, GL_STATIC_DRAW);
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LineEBO); // GL_ELEMENT_ARRAY_BUFFER �����������ι��ε� 
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

