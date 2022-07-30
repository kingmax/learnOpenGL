#pragma once

#include <glad/glad.h>
#include <glfw3.h>
// texture loader
#include "stb_image.h"
// math lib
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// data
//#include "data.h"
// shader
#include "shaderClass.h"
#include "camera.h"



#pragma region window

//������α�����main���涨�壬��Ҫ����init window��callback����
//const unsigned screenWidth = 800;
//const unsigned screenHeight = 600;
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = screenWidth / 2.0f;
//float lastY = screenHeight / 2.0f;
//bool isFirstMouse = true;
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;

extern Camera camera;
extern float lastX;
extern float lastY;
extern bool isFirstMouse;
extern float deltaTime;
extern float lastFrame;


void mouse_callback(GLFWwindow* win, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* win, double xoffset, double yoffset);

void framebuffer_size_callback(GLFWwindow* win, const int w, const int h);
void processInput(GLFWwindow* win);

void init(GLFWwindow*& win, const char* title=u8"ѧϰOpenGL", const int screenWidth=800, const int screenHeight=600, const int versionMajor=4, const int versionMinor=6);

void printMaxVASupport();

#pragma endregion window


// �ӳ������(CPU��)��Shaderȫ�ֱ����ṩ���� (02.frag�е�uniform vec4 ourColor;)
void sendColor2Shader(unsigned shaderProgram, const char* uniformShaderVariableName);


#pragma region PrepareData and DrawModel

// ����д����vertices����������������������޷������õ����ݣ������⹩�ο�����Ҫʹ�������ģ�庯������ģ�庯��Ϊ�˲��������ӱ�������Ҫ��������ʵ��д��һ���ļ���
void prepareTriangle(const float* vertices, unsigned& VAO, const unsigned stride=3);

// ��Ϊģ�庯������ʱ������ʵ�ʴ��룬�����������ʵ�ַ���ͬһ���ļ��У���main�е��ã�����ʱʱ�ͻ���ֱ���:"�޷��������ⲿ����"
// https://blog.csdn.net/qq_39779233/article/details/102973324
// ����������Ϊ������������������������д������������Ϊ����������Ϊָ�룬����һ��ָ���޷�ȷ�����鳤��
// https://bbs.csdn.net/topics/390362450
template<int N>
unsigned int prepareTriangleData(float(&vertices)[N])
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object, ���㻺��������ڹ���GPU�Դ��еĴ�����������
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy vertex data to GPU�� ��CPU��������������GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��GPU�ṩҪ��ν�����Щ���ݣ��������������һ���ֶ�Ӧvertex shader���ĸ��������ԣ� ����������ÿ3����ʾһ��position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //��һ������0����vert shader :: layout (location = 0)
	//���ö�������
	glEnableVertexAttribArray(0);
	return VAO;
}

// Ĭ�ϲ�����Ҫд��ͷ�ļ���
void drawTriangle(const unsigned& VAO, Shader& myShader, const unsigned vertexCount = 3);


template<int M, int N>
unsigned int prepareRectangleData(float(&vertices)[M], unsigned int(&indices)[N], const unsigned stride = 6)
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}

void drawRectangle(const unsigned& VAO, Shader& myShader, const unsigned vertexCount=6);

template<int M, int N>
unsigned int prepareRectangleWithUV(float(&vertices)[M], unsigned int(&indices)[N], const unsigned stride = 8)
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return VAO;
}


void drawRectangleWithTexture(const unsigned& VAO, Shader& myShader, const unsigned& texture, const unsigned vertexCount = 6);
void drawRectangleWithTextureMix(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const unsigned vertexCount = 6);
void drawRectangleWithTextureMixTransform(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const glm::mat4& trans, const unsigned vertexCount = 6);

template<int N>
unsigned int prepareBoxData(float(&vertices)[N])
{
	// VBO Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//send vertex data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//describe to shader
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	return VAO;
}

void drawBox(const unsigned& VAO, Shader& myShader, const unsigned vertexCount = 180);

void drawBox(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const unsigned vertexCount = 180);
// draw one box with position, ratation
void drawBox(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const glm::mat4 model, const unsigned vertexCount = 180);

glm::mat4 getNewModelMatrix(glm::vec3& position);

void drawBox10(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const unsigned vertexCount = 180);

void drawBox10(const unsigned& VAO, Shader& myShader, const unsigned vertexCount = 180);
#pragma endregion PrepareData and DrawModel


#pragma region Texture
unsigned loadTexture(const string textureFilename);
#pragma endregion Texture


#pragma region Transformation
void translateTest();
void genTransform(glm::mat4& trans);
void updateTransform(glm::mat4& trans);

// welcome to 3D
void getMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, const unsigned screenWidth, const unsigned screenHeight);
// send MVP matrix to vertex shader (3d.vert)
void updateMVP4Shader(Shader& myShader, glm::mat4& model, glm::mat4& view, glm::mat4& projection);

// camera (view Matrix)
glm::mat4 getAnimateView(const float radius = 10.0f);

// handle camera
glm::mat4 handleCameraView(GLFWwindow* win, glm::vec3& cameraPos, glm::vec3& cameraFront, const glm::vec3& cameraUp = glm::vec3(0.0f, 1.0f, 0.0f), const float speed = 0.05f);

void getDeltaTime(float& deltaTime, float& lastTime);



#pragma endregion Transformation