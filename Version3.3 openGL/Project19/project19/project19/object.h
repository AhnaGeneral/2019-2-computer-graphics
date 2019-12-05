#include"source.h"

GLuint  PyramidVBO[3], PyramidEBO;
GLuint  ShapeVBO[3], ShapeEBO;
unsigned int First_RectVAO, Second_RectVAO, Third_RectVAO, Ground_RectVAO, Light_VAO;


void InitBuffer();
void InitBuffer2();
void InitBuffer3();
void InitBuffer4();
void LightBuffer(); 

GLfloat Ground_Rect[] =
{
-10.0f, -0.2f, +10.0f,
-10.0f, -0.2f, -10.0f,
+10.0f, -0.2f, -10.0f,
+10.0f, -0.2f, +10.0f,
};
GLfloat First_Rect[] =
{
-0.5f, -0.2f, +0.5f,
-0.5f, -0.2f, -0.5f,
+0.5f, -0.2f, -0.5f,
+0.5f, -0.2f, +0.5f,
-0.5f, +0.2f, +0.5f,
-0.5f, +0.2f, -0.5f,
+0.5f, +0.2f, -0.5f,
+0.5f, +0.2f, +0.5f
};
GLfloat Second_Rect[] =
{
-0.3f, -0.0f, +0.3f,
-0.3f, -0.0f, -0.3f,
+0.3f, -0.0f, -0.3f,
+0.3f, -0.0f, +0.3f,
-0.3f, +0.3f, +0.3f,
-0.3f, +0.3f, -0.3f,
+0.3f, +0.3f, -0.3f,
+0.3f, +0.3f, +0.3f
};
GLfloat Third_Rect[] =
{
-0.1f, +0.0f, +0.1f,
-0.1f, +0.0f, -0.1f,
+0.1f, +0.0f, -0.1f,
+0.1f, +0.0f, +0.1f,
-0.1f, +0.5f, +0.1f,
-0.1f, +0.5f, -0.1f,
+0.1f, +0.5f, -0.1f,
+0.1f, +0.5f, +0.1f
};
unsigned int Ground_Index[] = { 0,1,2,  2,3,0 };
unsigned int First_Rect_Index[] = { 0,4,3, 3,4,7, 0,2,1, 3,2,1, 5,6,7, 5,7,4, 2,6,5, 2,5,1, 1,5,4, 1,4,0, 3,7,6, 3,6,2 };
unsigned int Second_Rect_Index[] ={ 0,4,3, 3,4,7, 0,2,1, 3,2,1, 5,6,7, 5,7,4, 2,6,5, 2,5,1, 1,5,4, 1,4,0, 3,7,6, 3,6,2 };
unsigned int Third_Rect_Index[] = { 0,4,3, 3,4,7, 0,2,1, 3,2,1, 5,6,7, 5,7,4, 2,6,5, 2,5,1, 1,5,4, 1,4,0, 3,7,6, 3,6,2 };

GLfloat Ground_Normal[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
GLfloat Ground_Colors[] = { 0.5f, 0.2f, 0.5f, 0.5f, 0.2f, 0.5f,0.5f, 0.2f, 0.5f, 0.5f, 0.2f, 0.5f };

GLfloat First_Rect_Colors[] = { 0.0 , 1.0, 0.0, 0.0 , 1.0, 0.0 , 0.0 , 1.0 , 0.0,  0.0 , 1.0, 0.0,  0.0 , 1.0, 0.0,  0.0 , 1.0, 0.0 ,0.0 , 1.0, 0.0 ,0.0 , 1.0, 0.0 };
GLfloat Second_Rect_Colors[] ={ 0.0 , 0.0 , 1.0  ,0.0 , 0.0 , 1.0 , 0.0 , 0.0 , 1.0,  0.0 , 0.0 , 1.0,  0.0 , 0.0 , 1.0,  0.0 , 0.0 , 1.0 ,0.0 , 0.0 , 1.0 ,0.0 , 0.0 , 1.0 };
GLfloat Third_Rect_Colors[] = {1.0 , 0.0 , 0.0  ,1.0 , 0.0 , 0.0  , 1.0 , 0.0 , 0.0 ,  1.0 , 0.0 , 0.0 ,  1.0 , 0.0 , 0.0,  1.0 , 0.0 , 0.0 ,1.0 , 0.0 , 0.0 ,1.0 , 0.0 , 0.0 };
float vertices[] = { 
	// 꼭지점속성: x, y, z, normalX, normalY, normalZ 
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,         
0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,      
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,      
0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f };

GLfloat verticesColor[] =
{
	1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f
};
void InitBuffer()
{
	glGenVertexArrays(1, &First_RectVAO);
	glGenBuffers(3, ShapeVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &ShapeEBO);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(First_Rect), First_Rect, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ShapeEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(First_Rect_Index), First_Rect_Index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(First_Rect_Colors), First_Rect_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(First_Rect_Colors), First_Rect_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

}
void InitBuffer2()
{
	glGenVertexArrays(1, &Second_RectVAO);
	glGenBuffers(3, PyramidVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &PyramidEBO);
	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Second_Rect), Second_Rect, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PyramidEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Second_Rect_Index), Second_Rect_Index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Second_Rect_Colors), Second_Rect_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Second_Rect_Colors), Second_Rect_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
}
void InitBuffer3()
{
	glGenVertexArrays(1, &Third_RectVAO);
	glGenBuffers(3, PyramidVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &PyramidEBO);
	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Third_Rect), Third_Rect, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PyramidEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Third_Rect_Index), Third_Rect_Index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Third_Rect_Colors), Third_Rect_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Third_Rect_Colors), Third_Rect_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
}
void InitBuffer4()
{
	glGenVertexArrays(1, &Ground_RectVAO);
	glGenBuffers(3, PyramidVBO);                  // 2개의 VBO지정하고 할당하기
	glGenBuffers(1, &PyramidEBO);
	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Ground_Rect), Ground_Rect, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PyramidEBO); // GL_ELEMENT_ARRAY_BUFFER 버퍼유형으로바인딩 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Ground_Index), Ground_Index, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Ground_Normal), Ground_Normal, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, PyramidVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Ground_Colors), Ground_Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
}

void LightBuffer()
{
	unsigned int VBO[2];
	glGenVertexArrays(1, &Light_VAO);
	glGenBuffers(2, VBO);
	glBindVertexArray(Light_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);                   //---위치속성
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //---노말속성
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColor), verticesColor, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

}