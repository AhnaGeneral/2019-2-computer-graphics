#include "stdafx.h"
#include "readshader.h"
#include "CGameframework.h"
#include "CGameobject.h"

//[no remove]========================================
GLuint ProgramID;
glm::vec3 cameraPos = glm::vec3(0.0f, 4.0f, 15.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 4.0f, 0.0f);
//===================================================

//===================================================
CCube* m_Robot;// ��, �� 2��, �ٸ� 2�� , ����, �� = �� 7��
float angle = 0.0f, g_posx = 0.0f, g_posy = 0.0f, g_posz = 0.0f;
float Frontviewangle = 0.0f; 
float g_rLeg = 0.0f, g_lLeg = 0.0f, g_rArm = 0.0f, g_lArm = 0.0f;
bool startmove = false;
float Tick = 0.0f; 
CStage* m_Stage;
CFrontView* mFrontStage;
//===================================================


CGameFrameWork::CGameFrameWork()
{
}

CGameFrameWork::~CGameFrameWork()
{
	delete[] m_Robot;
}

void CGameFrameWork::FrameAdvance(int argc, char** argv)
{
	glutInit(&argc, argv); //glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //���÷��� ��� ����
	glutInitWindowPosition(100, 100); //�������� ��ġ ����
	glutInitWindowSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT); //�������� ũ�� ����
	glutCreateWindow("using index buffer");//������ ����

	//[ GLEW �ʱ�ȭ �ϱ� ]=======================================================
	glewExperimental = GL_TRUE; //glew �ʱ�ȭ
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized\n";
	ProgramID = (Complie_Shaders());

	m_Robot = new CCube[7];
	m_Stage = new CStage[4];
	mFrontStage = new CFrontView;
	//m_Ccube2 = new CCube;

	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUP);
	glutTimerFunc(100, Timerfunction, 1);
	glutDisplayFunc(RenderScene); // ��� �Լ��� ���� 
	glutReshapeFunc(Reshape);
	//glutMouseFunc(MouseEvent);
	//glutReshapeFunc (); // �ٽ� �׸��� �Լ� ���� 
	glutMainLoop();

}

void RenderScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
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

	//[����]===========================================================
	glm::mat4 CenterBody = glm::mat4(1.0f); // ������ķ��ʱ�ȭ 
	CenterBody = glm::translate(CenterBody, glm::vec3(g_posx, g_posy, g_posz));
	cout << "g_posx : " << g_posx << "g_posy : " << g_posy << "g_posz : " << g_posz << endl; 
	CenterBody = glm::rotate(CenterBody,angle,glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(CenterBody));
	m_Robot[0].ChangeColor(0.4f,0.4f,0.0f);//����
	m_Robot[0].Render();

	//[��]============================================================
	glm::mat4 Head = glm::mat4(1.0f);
	Head = glm::translate(CenterBody, glm::vec3(0.0, 0.45, 0.0));
	Head = glm::scale(Head, glm::vec3(0.5, 0.5, 0.5));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Head));
	m_Robot[1].ChangeColor(0.2f, 0.2f, 0.2f);//
	m_Robot[1].Render();

	//[��]==============================================================
	Head = glm::translate(Head, glm::vec3(0.0, 0.0, 0.45));
	Head = glm::scale(Head, glm::vec3(0.2, 0.2, 0.5));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Head));
	m_Robot[2].ChangeColor(0.8f, 0.2f, 0.2f);//
	m_Robot[2].Render();

	//[������]===========================================================
	glm::mat4 RightArm = glm::mat4(1.0f);
	RightArm = glm::translate(CenterBody, glm::vec3(0.36f, +0.1f, 0.0f));
	RightArm = glm::rotate(RightArm, g_rArm, glm::vec3(1.0f, 0.0f, 0.0f));
	RightArm = glm::scale(RightArm, glm::vec3(0.2, 0.7, 0.2));
	RightArm = glm::translate(RightArm, glm::vec3(0.0f, -0.18f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(RightArm));
	m_Robot[3].ChangeColor(1.0f,0.0f,0.5f);//
	m_Robot[3].Render();

	//[����]============================================================
	glm::mat4 LeftArm = glm::mat4(1.0f);
	LeftArm = glm::translate(CenterBody, glm::vec3(-0.36f, +0.10f, 0.0f));
	LeftArm = glm::rotate(LeftArm, g_lArm, glm::vec3(1.0f, 0.0f, 0.0f));
	LeftArm = glm::scale(LeftArm, glm::vec3(0.2, 0.7, 0.2));
	LeftArm = glm::translate(LeftArm, glm::vec3(0.0f, -0.18f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(LeftArm));
	m_Robot[4].ChangeColor(0.1f, 0.6f, 0.8f);//
	m_Robot[4].Render();

	//[�����ٸ�]========================================================
	glm::mat4 RightLeg = glm::mat4(1.0f);
	RightLeg = glm::translate(CenterBody, glm::vec3(0.15f, -0.38f, 0.0f));
	RightLeg = glm::rotate(RightLeg, g_rLeg, glm::vec3(1.0f, 0.0f, 0.0f));
	RightLeg = glm::scale(RightLeg, glm::vec3(0.3, 0.7, 0.3));
	RightLeg = glm::translate(RightLeg, glm::vec3(0.0f, -0.15f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(RightLeg));
	m_Robot[5].ChangeColor(1.0f, 1.0f, 0.0f);//
	m_Robot[5].Render();

	//[�޴ٸ�]==========================================================
	glm::mat4 LeftLeg = glm::mat4(1.0f);
	LeftLeg = glm::translate(CenterBody, glm::vec3(-0.15f, -0.38f, 0.0f));
	LeftLeg = glm::rotate(LeftLeg, g_lLeg, glm::vec3(1.0f, 0.0f, 0.0f));
	LeftLeg = glm::scale(LeftLeg, glm::vec3(0.3, 0.7, 0.3));
	LeftLeg = glm::translate(LeftLeg, glm::vec3(0.0f, -0.15f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(LeftLeg));
	m_Robot[6].ChangeColor(0.2f, 0.9f, 0.4f);//
	m_Robot[6].Render();
	
	glm::mat4 groung = glm::mat4(1.0f);
	groung = glm::translate(groung, glm::vec3(0.0f, +1.5f, 0.0f));
	groung = glm::scale(groung, glm::vec3(20.0, 8.0, 20.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(groung));
	m_Stage[0].SetColor(0.6);
	m_Stage[0].InitBuffer();
	m_Stage[0].Render();


	glm::mat4 FrontStage = glm::mat4(1.0f);
	FrontStage = glm::translate(FrontStage, glm::vec3(0.0f, +1.5f, 0.0f));
	FrontStage = glm::scale(FrontStage, glm::vec3(20.0, 8.0, 20.0));
	FrontStage = glm::rotate(FrontStage, Frontviewangle, glm::vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(FrontStage));
	mFrontStage->InitBuffer(); 
	mFrontStage->Render(); 

	glutSwapBuffers();
}
bool frontUP = false;
void Keyboard(unsigned char key, int x, int y)
{

	if (key == 'o')
	{//Frontviewangle -= 0.05f;
		frontUP = true;
	}
	//[ �̵�Ű ]==================
	if (key == 'w' || key == 'W')
	{
		if (g_posz < -4.9)
			g_posz += 0.0f; 
		else {
			angle = 3.2f;
			g_posz -= 0.1f;
			startmove = true;
			Tick = 0.4;
		}
		//if (g_posx > 5.3 || g_posx < -5.3)
		//	g_posx += 0.0f;
	}
	if (key == 's' || key == 'S')
	{
		angle = 0.0f;
		g_posz += 0.1f;
		startmove = true;
		Tick = 0.4;
	}
	if (key == 'a' || key == 'A')
	{
		if (g_posx < -5.3)
			g_posx += 0.0f;
		else
		{
			angle = -1.6f;
			g_posx -= 0.1f;
			startmove = true;
			Tick = 0.4;
		}
	}
	if (key == 'd' || key == 'D')
	{
		if (g_posx > 5.3)
			g_posx += 0.0f;
		else {
			angle = 1.6f;
			g_posx += 0.1f;
			startmove = true;
			Tick = 0.4;
		}
	}//============================

	if (key == ' ')
	{
		//angle = 1.6f;
		g_posy += 0.4f;
		startmove = false; 
	}
	glutPostRedisplay();
}

bool RotateRateCheck = false;

void Timerfunction(int value)
{
	if (Frontviewangle <= -1.5)
		frontUP = false;
	if (startmove)
	{
		if (RotateRateCheck == false)
		{
			g_rArm += Tick;
			g_lArm -= Tick;
			g_rLeg -= Tick;
			g_lLeg += Tick; 
			if (g_rArm > 1.0f) { RotateRateCheck = true; }
		}
		else if (RotateRateCheck == true)
		{
			g_rArm -= Tick;
			g_lArm += Tick;
			g_rLeg += Tick;
			g_lLeg -= Tick;
			if (g_rArm < -1.0f) { RotateRateCheck = false; }
		}
	}

	if(frontUP)
	Frontviewangle -= 0.05f;

	glutTimerFunc(100, Timerfunction, 1);
	glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y)
{

}

void KeyboardUP(unsigned char key, int x, int y)
{
	startmove = false;
	if (key == ' ')
	{
		for (int i = 0; i < 4; ++i)
		{
			g_posy -= 0.1f;
		}
	}
	
	glutPostRedisplay();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glutPostRedisplay();
}
