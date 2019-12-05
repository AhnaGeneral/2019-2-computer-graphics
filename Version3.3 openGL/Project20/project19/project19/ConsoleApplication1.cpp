#include "object.h"
#include "loadObj.h"
#include <vector>
#include <atlfile.h>
#include <stdlib.h>
//=========================================
struct POSITION
{
	float x, y, z; 
};

struct INFO_OBJECT
{
	std::vector< glm::vec3 > outvertex; 
	std::vector< glm::vec3 > outnormal;
	std::vector< glm::vec2 > outuv;
};
//=========================================

void drawScene();
void Keyboard(unsigned char key, int x, int y);
void spherebufffers(); 
void DefaultTimerFunction(int);
GLuint pid;

float CameraX = 0.0f, CameraZ = 0.0f, CameraY = 0.0f;
float angle = 90.0f,  radius = 5.0f;
float rotateangle= 0.0f; 
int size_cube, size_sphere; 
float sunRotate = 0.0f;
float earthRotate = 0.0f;

unsigned int sphereVAO, sphereVBO[3];
POSITION s_posLight{4.0f,4.0f,0.0f}, s_posColor{ 1.0, 1.0, 1.0 };
int sizecube, sizesphere;
INFO_OBJECT info_sphere;

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

	sizesphere = loadObj("sphereTest.obj", info_sphere.outvertex,info_sphere.outnormal, info_sphere.outuv);
	glutKeyboardFunc(Keyboard);
	//glutMouseFunc(MouseEvent);
	glutTimerFunc(100, DefaultTimerFunction, 0);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutMainLoop();
}


void spherebufffers()
{
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(3, sphereVBO);  // 2개의 VBO지정하고 할당하기

	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizesphere* sizeof(glm::vec3), &info_sphere.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizesphere * sizeof(glm::vec3), &info_sphere.outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizesphere * sizeof(glm::vec3), &info_sphere.outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
}

void drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(pid);
	//회전값에 대한 검사====================================
	
	glm::vec3 cameraPos = glm::vec3(0.0f+CameraX , 0.0f, 0.0f+CameraZ+ 20.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f + CameraX, 1.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f , 1.0f,  0.0f);

	int modelLoc = glGetUniformLocation(pid, "model");
	int viewLoc = glGetUniformLocation(pid, "view");
	int projLoc = glGetUniformLocation(pid, "projection");
	int camerapos = glGetUniformLocation(pid, "viewPos");
	glUniform3f(camerapos,cameraPos.r, cameraPos.g, cameraPos.b );
	
    int lightPosLocation = glGetUniformLocation(pid, "lightPos");
	glm::vec4 InitLightPos = glm::vec4(4.0f, 4.0f, 4.0f, 1.0f);
	glm::mat4 destLight = glm::rotate(glm::mat4(1.0f) , rotateangle, glm::vec3(0.0f, 1.0f, 0.0f));
	InitLightPos= destLight*InitLightPos;
	glUniform3f(lightPosLocation, InitLightPos.r, InitLightPos.g, InitLightPos.b);
	
	int lightColorLocation = glGetUniformLocation(pid, "lightColor");
	glUniform3f(lightColorLocation, s_posColor.x, s_posColor.y, s_posColor.z);

	int objColorLocation = glGetUniformLocation(pid, "objectColor");
	glUniform3f(objColorLocation, 0.5, 0.5, 0.5);

	//변환행렬초기화 ========================================================
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	for (auto degree : { 0.0f, 45.0f, 135.0f })
	{
		auto sunMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));
		sunMatrix = glm::translate(sunMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		sunMatrix = glm::rotate(sunMatrix, glm::radians(sunRotate+degree/5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		sunMatrix = glm::rotate(sunMatrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		sunMatrix = glm::scale(sunMatrix, glm::vec3(0.125f, 0.125f, 0.125f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sunMatrix));
		if (!degree)
		{
			glBindVertexArray(sphereVAO);
			spherebufffers();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, sizesphere);
			//glDeleteBuffers(3, sphereVBO);
		}

		auto earthmatrix = glm::rotate(sunMatrix, -angle, glm::vec3(1.0f, 0.0f, 0.0f));
		earthmatrix = glm::translate(earthmatrix, glm::vec3(50.0f, 0.0f, 0.0f));
		earthmatrix = glm::rotate(earthmatrix, glm::radians(earthRotate + (degree / 45.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		earthmatrix = glm::rotate(earthmatrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		earthmatrix = glm::scale(earthmatrix, glm::vec3(0.45f, 0.45f, 0.45));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(earthmatrix));
		glBindVertexArray(sphereVAO);
		spherebufffers();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, sizesphere);
		//glDeleteBuffers(3, sphereVBO);		


		auto moonMatrix = glm::rotate(earthmatrix, -angle, glm::vec3(1.0f, 0.0f, 0.0f));
		moonMatrix = glm::translate(earthmatrix, glm::vec3(40.0f, 0.0f, 0.0f));
		moonMatrix = glm::rotate(moonMatrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		moonMatrix = glm::scale(moonMatrix, glm::vec3(0.3f, 0.3f, 0.3));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(moonMatrix));
		glBindVertexArray(sphereVAO);
		spherebufffers();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, sizesphere);
		//glDeleteBuffers(3, sphereVBO);			//gluDisk(objDisk, 1.95, 2.0, 20, 1);
	}


	////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(Light_VAO);
	LightBuffer(); 
	glm::mat4 LightTrans = glm::mat4(1.0f); 
	LightTrans = glm::rotate(LightTrans, rotateangle, glm::vec3(0.0f, 1.0f, 0.0f)); 
	LightTrans = glm::translate(LightTrans, glm::vec3(s_posLight.x, s_posLight.y, s_posLight.z));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(LightTrans));

	glDrawArrays(GL_TRIANGLES, 0, 36); 
	glDeleteBuffers(1, &Light_VAO);

	glutSwapBuffers();
}

bool LightRotateBool = false;
bool SunRotateBool = false; 
void Keyboard(unsigned char key, int x, int y)
{

	if (key == 'z') { CameraZ -= 1.0f; }
	if (key == 'Z') { CameraZ += 1.0f; }
	if (key == 'x') { CameraX -= 1.0f; }
	if (key == 'X') { CameraX += 1.0f; }

	if (key == 'r') { LightRotateBool = true; }
	if (key == 'R') { LightRotateBool = false; }

	if (key == 'a') { s_posLight.x -= 0.2; }
	if (key == 'd') { s_posLight.x += 0.2; }
	if (key == 's') { s_posLight.y += 0.2; }
	if (key == 'w') { s_posLight.y -= 0.2; }

	if (key == 'o') 
	{
		s_posColor.x = 0.1;
		s_posColor.y = 0.1;
		s_posColor.z = 0.1;
	}
	if (key == 'O')
	{
		s_posColor.x = 0.9;
		s_posColor.y = 0.9;
		s_posColor.z = 0.9;
	}

	if (key == 'c')
	{
		s_posColor.x = rand() / (float)RAND_MAX;
		s_posColor.y = rand() / (float)RAND_MAX;
		s_posColor.z = rand() / (float)RAND_MAX;
	}
}

void DefaultTimerFunction(int value)
{
	if (LightRotateBool)
		rotateangle += 0.2f;

	sunRotate += 5.f; 
	earthRotate += 10.0f; 

	glutTimerFunc(100, DefaultTimerFunction, 0); // 타이머함수재설정
	glutPostRedisplay(); // 화면재출력
}
