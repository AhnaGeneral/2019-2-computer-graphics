#include "source.h"
#include <ctime>

void drawScene();
//void Keyboard(unsigned char key, int x, int y);
//void SpecialKeyboard(int key, int x, int y);
void InitBuffer();
unsigned int ShapeVAO, PyramidVAO;

void DefaultTimerFunction(int);

GLfloat TriShape[] =
{
-0.5f, -0.5f, -0.5f,  -0.5f, -0.5f, +0.5f, -0.5f, +0.5f, -0.5f,
-0.5f, +0.5f, +0.5f, +0.5f, -0.5f, -0.5f,  +0.5f, -0.5f, +0.5f,
+0.5f, +0.5f, -0.5f,  +0.5f, +0.5f, +0.5f
};

GLfloat TriColors[] = 
{ 0.0f, 1.0f, 1.0f,  0.0f , 1.0f ,1.0f,
   0.0f, 1.0f, 1.0f,  0.0f , 1.0f ,1.0f,
   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
   0.0f, 1.0f, 1.0f,  0.0f , 1.0f ,1.0f };

unsigned int Triindex[] =
{ 2,6,4, 0,2,4, 5,6,7, 
  5,4,6, 2,7,6, 2,3,7,
  3,2,1,  2,0,1,  5,7,3,
  5,3,1,  0,4,5, 0,5,1 };

glm::vec3 cuberotation0[] =
{
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
};

glm::vec3 cubePositions0[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(7.0f,  0.5f, 0.0f),
  glm::vec3(-2.0f,  0.5f, 0.0f),
  glm::vec3(0.0f,  0.0f,  -5.0f),
  glm::vec3(0.0f,  1.0f, -5.0f)
};

glm::vec3 cuberotation1[] =
{
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f)
};


glm::vec3 cubePositions1[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f)
};

glm::vec3 cuberotation2[] =
{
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  1.0f,  0.0f),
};

glm::vec3 cubePositions2[] = {
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(2.0f,  0.5f, 0.0f),
  glm::vec3(7.0f,  0.5f, 0.0f),
  glm::vec3(-2.0f,  0.5f, 0.0f),
  glm::vec3(0.0f,  0.0f,  -5.0f),
};

glm::vec3 cuberotation3[] =
{
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
};

glm::vec3 cubePositions3[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(2.0f,  0.5f, 0.0f),
  glm::vec3(7.0f,  0.5f, 0.0f),
  glm::vec3(-2.0f,  0.5f, 0.0f),
};

glm::vec3 cuberotation4[] =
{
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
};

glm::vec3 cubePositions4[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(2.0f,  0.5f, 0.0f),
  glm::vec3(0.0f,  1.0f, -5.0f)
}; 

glm::vec3 cuberotation5[] =
{
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
};

glm::vec3 cubePositions5[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  0.5f, 0.0f),
  glm::vec3(7.0f,  0.5f, 0.0f),
  glm::vec3(-2.0f,  0.5f, 0.0f),
  glm::vec3(0.0f,  1.0f, -5.0f)
};

glm::vec3 cuberotation6[] =
{
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
};

glm::vec3 cubePositions6[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  0.5f, 0.0f),
  glm::vec3(7.0f,  0.5f, 0.0f),
  glm::vec3(-2.0f,  0.5f, 0.0f),
  glm::vec3(0.0f,  0.0f,  -5.0f),
  glm::vec3(0.0f,  1.0f, -5.0f)
};

glm::vec3 cuberotation7[] =
{
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
};

glm::vec3 cubePositions7[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(7.0f,  0.5f, 0.0f),
  glm::vec3(0.0f,  1.0f, -5.0f)
};

glm::vec3 cuberotation9[] =
{
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
};

glm::vec3 cubePositions9[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(2.0f,  0.5f, 0.0f),
  glm::vec3(7.0f,  0.5f, 0.0f),
  glm::vec3(0.0f,  1.0f, -5.0f)
};

glm::vec3 cuberotation8[] =
{
  glm::vec3(0.0f,  1.0f,  0.0f), 
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(0.0f,  0.0f, 1.0f), 
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  0.0f, 1.0f),
  glm::vec3(0.0f,  1.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
};

glm::vec3 cubePositions8[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(0.0f,  1.0f, 0.0f),
  glm::vec3(2.0f,  0.5f, 0.0f),  
  glm::vec3(7.0f,  0.5f, 0.0f), 
  glm::vec3(-2.0f,  0.5f, 0.0f),
  glm::vec3(0.0f,  0.0f,  -5.0f),
  glm::vec3(0.0f,  1.0f, -5.0f)
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

	//glutKeyboardFunc(Keyboard);
	//glutSpecialFunc(SpecialKeyboard); 
	//glutMouseFunc(MouseEvent);
	glutTimerFunc(1, DefaultTimerFunction, 0);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutMainLoop();
}

void drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(Complie_Shaders());

	glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 5.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	int modelLoc = glGetUniformLocation(Complie_Shaders(), "model");
	int viewLoc = glGetUniformLocation(Complie_Shaders(), "view");
	int projLoc = glGetUniformLocation(Complie_Shaders(), "projection");
	
	//[ 좌표축 만들기 ]////////////////////////////////////////////////////////////////////////
	glBindVertexArray(ShapeVAO); // VAO를 바인드하기
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
 
	for (int i = 0; i < 6; ++i)
	{
		glm::mat4 Matrix = glm::mat4(1.0f);
		Matrix = glm::rotate(Matrix, glm::radians(90.0f), cuberotation6[i]);
		//Matrix = glm::rotate(Matrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		Matrix = glm::scale(Matrix, glm::vec3(0.1f, 0.5f, 0.1f));
		Matrix = glm::translate(Matrix, cubePositions6[i]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Matrix));
		InitBuffer();
		//glPolygonMode(GL_FRONT, GL_FILL);
		glDrawElements(GL_LINE_LOOP, 36, GL_UNSIGNED_INT, 0);
	}

	for (int i = 0; i < 5; ++i)
	{
		glm::mat4 Matrix = glm::mat4(1.0f);
		Matrix = glm::rotate(Matrix, glm::radians(90.0f), cuberotation5[i]);
		//Matrix = glm::rotate(Matrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		Matrix = glm::scale(Matrix, glm::vec3(0.1f, 0.5f, 0.1f));
		Matrix = glm::translate(Matrix, cubePositions5[i]);
		//Matrix = glm::translate(Matrix, glm::vec3(5.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Matrix));
		InitBuffer();
		//glPolygonMode(GL_FRONT, GL_FILL);
		glDrawElements(GL_LINE_LOOP, 36, GL_UNSIGNED_INT, 0);
	}
	time_t curr_time;  // long과 동일
	struct tm* curr_tm; // 시간 표시를 위한 구조체 선언
	curr_time = time(NULL); //time 함수는 1970년 1월 1일 이후 몇초가 지났는지 계산한다.
	
	curr_tm = localtime(&curr_time); //지역 시간을 기준으로 변환 및 출력 편의를 위하여 tm 구조체에 저장함. 

	cout << curr_tm->tm_hour << "시" << curr_tm->tm_min << "분" << curr_tm->tm_sec << "초" << endl; 

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();

	glutSwapBuffers();
}

void DefaultTimerFunction(int value)
{
	glutPostRedisplay(); // 화면재출력
	glutTimerFunc(1, DefaultTimerFunction, 0); // 타이머함수재설정
}

void InitBuffer()
{
	glGenVertexArrays(1, &PyramidVAO);
	GLuint  ShapeVBO[2], ShapeEBO;
	glGenBuffers(2, ShapeVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &ShapeEBO);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriShape), TriShape, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ShapeEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Triindex), Triindex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriColors), TriColors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
}