#include "object.h"
#include <vector>
#include <atlfile.h>

void drawScene();
void Keyboard(unsigned char key, int x, int y);

int modelnumber = 1;
float RotateY = 0.0f, RotateX = 0.0f, RotateZ = 0.0f;
float CameraX = 0.0f, CameraZ = 0.0f, CameraY = 0.0f;
float angle = 0.0f, fAngle = 0.0f, radius = 5.0f, Tick = 6.0;
float CenterXRotate = 0.0f;
float CenterZRotate = 0.0f;
bool MovingX = false, MovingY = false, MovingZ = false;
bool Movingx = false, Movingy = false, Movingz = false;
bool RotateX_Check = false, RotateZ_Check = false;
void DefaultTimerFunction(int);
GLuint pid;
float rotateangle= 0.0f; 
int size_cube, size_sphere; 
struct POSITION
{
	float x, y, z; 
};

std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< glm::vec3 > temp_vertices;
std::vector< glm::vec2 > temp_uvs;
std::vector< glm::vec3 > temp_normals;
std::vector< glm::vec3 > outvertex, outnormal;
std::vector< glm::vec2 > outuv;

int loadObj(const char* filename);
float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
float aveX, aveY, aveZ;
float scaleX, scaleY, scaleZ;
float minX = -20.0, minY = -20.0, minZ = -20.0;
float maxX = 20.0, maxY = 20.0, maxZ = 20.0;
float scaleAll;

POSITION s_posLight{4.0f,4.0f,0.0f}, s_posColor{ 1.0,1.0, 1.0 };


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
	size_cube=loadObj("Cube.obj");
	glutKeyboardFunc(Keyboard);
	//glutMouseFunc(MouseEvent);
	glutTimerFunc(100, DefaultTimerFunction, 0);
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutMainLoop();
}

void drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(pid);
	//회전값에 대한 검사====================================
	{
		if (RotateY == 360.0f || RotateY == -360.0f) RotateY = 0.0f;
		if (Movingx == true) 
		{
			if (RotateX_Check == false) {
				RotateX += Tick;
				if (RotateX > 90.0f) { RotateX_Check = true; }
			}
			else if (RotateX_Check == true) {
				RotateX -= Tick;
				if (RotateX < -90.0f) { RotateX_Check = false; }
			}
		}
		if (Movingz == true) {
			if (RotateZ_Check == false) {
				RotateZ += Tick;
				if (RotateZ > 90.0f) { RotateZ_Check = true; }
			}
			else if (RotateZ_Check == true) {
				RotateZ -= Tick;
				if (RotateZ < -90.0f) { RotateZ_Check = false; }
			}
		}
		if (MovingY == true) RotateY += 3.0f;
		else if (Movingy == true)RotateY -= 3.0f;
	}
	
	glm::vec3 cameraPos = glm::vec3(0.0f+CameraX , 0.0f, 0.0f+CameraZ+ 10.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f + CameraX, 1.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f , 1.0f,  0.0f);

	int modelLoc = glGetUniformLocation(pid, "model");
	//cout << "modelLoc " << modelLoc << endl;

	int viewLoc = glGetUniformLocation(pid, "view");
	//cout << "viewLoc " << viewLoc << endl;

	int projLoc = glGetUniformLocation(pid, "projection");
	//cout << "projLoc " << projLoc << endl;

	//float cameraX = sinf(rotateangle)*4.0f;
	//float cameraZ = cosf(rotateangle)*4.0f;
	//float cameraY = s_posLight.y; //4.0f;

	int camerapos = glGetUniformLocation(pid, "viewPos");
	glUniform3f(camerapos,cameraPos.r, cameraPos.g, cameraPos.b );
	//cout << "camerapos"<< camerapos <<endl;
	
    int lightPosLocation = glGetUniformLocation(pid, "lightPos");
	glm::vec4 InitLightPos = glm::vec4(4.0f, 4.0f, 4.0f, 1.0f);
	glm::mat4 destLight = glm::rotate(glm::mat4(1.0f) , rotateangle, glm::vec3(0.0f, 1.0f, 0.0f));
	InitLightPos= destLight*InitLightPos;
	glUniform3f(lightPosLocation, InitLightPos.r, InitLightPos.g, InitLightPos.b);
	

	int lightColorLocation = glGetUniformLocation(pid, "lightColor");
	glUniform3f(lightColorLocation, s_posColor.x, s_posColor.y, s_posColor.z);
	//cout << "lightColorLocation" << lightColorLocation << endl;

	int objColorLocation = glGetUniformLocation(pid, "objectColor");
	glUniform3f(objColorLocation, 0.5, 0.5, 0.5);
	//cout << "objColorLocation" << objColorLocation << endl;

	//변환행렬초기화 ========================================================
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_SIZE_WIDTH / (float)WINDOW_SIZE_HEIGHT, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
	glm::mat4 Matrix = glm::mat4(1.0f);

	Matrix = glm::scale(Matrix, glm::vec3(3.0, 3.0, 3.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Matrix));
	//땅 ====================================================================
	glBindVertexArray(Ground_RectVAO);
	InitBuffer4();
	glm::mat4 modeltrans = glm::mat4(1.0f);
	modeltrans = glm::translate(modeltrans, glm::vec3(0.0f, -2.0f, 0.0f));
	modeltrans = glm::rotate(modeltrans,angle, glm::vec3(0.0f, 1.0f, 0.0f)); 
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modeltrans));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDeleteBuffers(3, PyramidVBO);
	glDeleteBuffers(1, &PyramidEBO);
	//첫번째 ==================================================================
	glBindVertexArray(First_RectVAO);
	InitBuffer();
	glm::mat4 model1trans = modeltrans;
	//model1trans = glm::scale(model1trans, glm::vec3(3.0, 3.0, 3.0));
	model1trans = glm::rotate(model1trans, glm::radians(RotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1trans));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDeleteBuffers(3, ShapeVBO);
	glDeleteBuffers(1, &ShapeEBO);
	//두번째 ================================================================
	glBindVertexArray(Second_RectVAO);
	InitBuffer2();
	glm::mat4 model2trans = model1trans;
	model2trans = glm::translate(model2trans, glm::vec3(0.0f, 0.3f, 0.0f));
	model2trans = glm::rotate(model2trans, glm::radians(RotateX), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2trans));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDeleteBuffers(3, PyramidVBO);
	glDeleteBuffers(1, &PyramidEBO);
	//세번째 ================================================================
	glBindVertexArray(Third_RectVAO);
	InitBuffer3();
	glm::mat4 model3trans = model2trans;
	model3trans = glm::translate(model3trans, glm::vec3(0.0f, 0.25f, 0.0f));
	model3trans = glm::rotate(model3trans, glm::radians(RotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3trans));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDeleteBuffers(3, PyramidVBO);
	glDeleteBuffers(1, &PyramidEBO);
	//=======================================================================

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

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'b') { Movingy = true; MovingY = false; }
	if (key == 'B') { Movingy = false; MovingY = true; }
	if (key == 'S'|| key == 's') 
	{
		MovingX = false;
		Movingx = false;
		MovingY = false;
		Movingy = false;
		MovingZ = false;
		Movingz = false;
	}
	if (key == 'm') { Movingx = true; MovingX = false; }
	if (key == 'M') { Movingx = false; MovingX = true; }
	if (key == 't') { Movingz = true; MovingZ = false; }
	if (key == 'T') { Movingz = false; MovingZ = true; }

	if (key == 'C' || key == 'c') { RotateX = 0; RotateY = 0; RotateZ = 0;
	angle += 1; 
	};

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

	

}

void DefaultTimerFunction(int value)
{
	
	if (LightRotateBool)
		rotateangle += 0.2f;
	glutTimerFunc(100, DefaultTimerFunction, 0); // 타이머함수재설정
	glutPostRedisplay(); // 화면재출력
}

int loadObj(const char* filename)
{
	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;

			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	aveX = sumX / vertexIndices.size();
	aveY = sumY / vertexIndices.size();
	aveZ = sumZ / vertexIndices.size();
	scaleX = (1.0 - maxX) * 10 + 1;
	scaleY = (1.0 - maxY) * 10 + 1;
	scaleZ = (1.0 - maxZ) * 10 + 1;

	if (scaleX > scaleY) {
		if (scaleY > scaleZ)
			scaleAll = scaleZ;
		else
			scaleAll = scaleY;
	}
	else if (scaleX < scaleY) {
		if (scaleX < scaleZ)
			scaleAll = scaleX;
		else
			scaleAll = scaleZ;
	}


	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		outvertex.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 vertex = temp_uvs[uvIndex - 1];
		outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_normals[normalIndex - 1];
		outnormal.push_back(vertex);
	}

	return outvertex.size();
}