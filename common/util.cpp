#include "util.h"

#include <iostream>
using namespace std;


void framebuffer_size_callback(GLFWwindow* win, const int w, const int h)
{
	cout << "size changed: " << w << " x " << h << endl;
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* win)
{
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, true);
	}

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	//cout << "cam Pos:[" << camera.Position.x << "," << camera.Position.y << "," << camera.Position.z << "] Yaw:" << camera.Yaw << ", Pitch:" << camera.Pitch << endl;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (isFirstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		isFirstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void init(GLFWwindow* &win, const char* title/* = u8"ѧϰOpenGL"*/, const int screenWidth/* = 800*/, const int screenHeight/* = 600*/, const int versionMajor/* = 4*/, const int versionMinor/* = 6*/)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// transparent ͸�� (shader::03.frag::myAlphaFromCPU)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	// �ޱ߿�
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	//win = glfwCreateWindow(screenWidth, screenHeight, u8"ѧϰOpenGL", NULL, NULL);
	win = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
	if (!win)
	{
		cerr << "���� GLFW Window ʧ��" << endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(win);

	//glViewport(0, 0, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
	// mouse move handle camera
	glfwSetCursorPosCallback(win, mouse_callback);
	// mouse scroll handle camera fov
	glfwSetScrollCallback(win, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cerr << "GLAD��ʼ��ʧ��" << endl;
		return;
	}

	glEnable(GL_DEPTH_TEST);

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void printMaxVASupport()
{
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;
}

/// <summary>
/// 
/// </summary>
/// <param name="shader">Shader Program Object</param>
/// <param name="uniformShaderVariableName"></param>
void sendColor2Shader(unsigned shader, const char* uniformShaderVariableName)
{
	float _time = (float)glfwGetTime();
	float green = sin(_time) / 2.0f;
	int variable_location = glGetUniformLocation(shader, uniformShaderVariableName);
	glUseProgram(shader);
	glUniform4f(variable_location, 0.0f, green, 0.0f, 1.0f);
}

// ����д����vertices����������������������޷������õ����ݣ������⹩�ο�����Ҫʹ�������ģ�庯������ģ�庯��Ϊ�˲��������ӱ�������Ҫ��������ʵ��д��һ���ļ���
void prepareTriangle(const float* vertices, unsigned &VAO, const unsigned stride/*=3*/)
{
	/*float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
	};*/

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object, ���㻺��������ڹ���GPU�Դ��еĴ�����������
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy vertex data to GPU�� ��CPU��������������GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��GPU�ṩҪ��ν�����Щ���ݣ��������������һ���ֶ�Ӧvertex shader���ĸ��������ԣ� ����������ÿ3����ʾһ��position
	glVertexAttribPointer(0, stride, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0); //��һ������0����vert shader :: layout (location = 0)
	//���ö�������
	glEnableVertexAttribArray(0); 

	//glUseProgram(shader);

	// ��VBO�洢��VAO�У��Ա�֮��ʹ�� ref: https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
	/*�����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO�С������ĺô����ǣ������ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO�����ˡ���ʹ�ڲ�ͬ�������ݺ���������֮���л���÷ǳ��򵥣�ֻ��Ҫ�󶨲�ͬ��VAO�����ˡ��ո����õ�����״̬�����洢��VAO��*/
	// �Ӱ�֮��������Ӧ�ð󶨺����ö�Ӧ��VBO������ָ�룬֮����VAO��֮��ʹ�á������Ǵ������һ�������ʱ������ֻҪ�ڻ�������ǰ�򵥵ذ�VAO�󶨵�ϣ��ʹ�õ��趨�Ͼ�����
	// �Ȱ�VAO, ע�����VAO�Ǹ�����ֵ���������ⲿ����
	//glBindVertexArray(VAO);
}

//template<int N>
//unsigned int prepareTriangleData(float(&vertices)[N])
//{
//	unsigned int VAO;
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	//Vertex Buffer Object, ���㻺��������ڹ���GPU�Դ��еĴ�����������
//	unsigned int VBO;
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	//copy vertex data to GPU�� ��CPU��������������GPU
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	//��GPU�ṩҪ��ν�����Щ���ݣ��������������һ���ֶ�Ӧvertex shader���ĸ��������ԣ� ����������ÿ3����ʾһ��position
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //��һ������0����vert shader :: layout (location = 0)
//	//���ö�������
//	glEnableVertexAttribArray(0);
//	return VAO;
//}

// in rendering loop, ��Ҫ������Ⱦѭ����
void drawTriangle(const unsigned& VAO, Shader& myShader, const unsigned vertexCount)
{
	//glUseProgram(myShader2.ID);
	myShader.use();
	sendColor2Shader(myShader.ID, "ourColor");
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void drawRectangle(const unsigned& VAO, Shader& myShader, const unsigned vertexCount)
{
	myShader.use();
	myShader.setFloat("myAlphaFromCPU", (float)sin(glfwGetTime()) / 2.0f + 0.5f);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

void drawRectangleWithTexture(const unsigned& VAO, Shader& myShader, const unsigned& texture, const unsigned vertexCount /*= 6*/)
{
	myShader.use();
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

void drawRectangleWithTextureMix(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const unsigned vertexCount /*= 6*/)
{
	// �����������Ԫ (Ĭ���Զ��󶨵�1����GL_TEXTURE0, ���һ����16������)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	myShader.use();
	// ��Ҫָ���ĸ�sampler2D��Ӧ�ĸ�������Ԫ
	glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0);
	myShader.setInt("texture2", 1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

void drawRectangleWithTextureMixTransform(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const glm::mat4& trans, const unsigned vertexCount /*= 6*/)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	myShader.use();
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1);
	unsigned transformLocation = glGetUniformLocation(myShader.ID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

void drawBox(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const unsigned vertexCount /*= 180*/)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	myShader.use();
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

// draw one box with position, ratation
void drawBox(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const glm::mat4 model, const unsigned vertexCount /*= 180*/)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	myShader.use();
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1);
	myShader.setMat4("model", model);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void drawBox(const unsigned& VAO, Shader& myShader, const unsigned vertexCount /*= 180*/)
{
	myShader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

glm::mat4 getNewModelMatrix(glm::vec3& position)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	return model;
}

void drawBox10(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const unsigned vertexCount /*= 180*/)
{
	glm::vec3 cubePositions[] = {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	for (unsigned i=0; i<10; i++)
	{
		glm::mat4 model = getNewModelMatrix(cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle) * (float)sin(glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));
		drawBox(VAO, myShader, texture1, texture2, model, vertexCount);
	}
}

void drawBox10(const unsigned& VAO, Shader& myShader, const unsigned vertexCount /*= 180*/)
{
	glm::vec3 cubePositions[] = {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	for (unsigned i=0; i<10; i++)
	{
		glm::mat4 model = getNewModelMatrix(cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle) * (float)sin(glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));
		myShader.use();
		myShader.setMat4("model", model);
		drawBox(VAO, myShader, vertexCount);
	}
}

unsigned loadTexture(const string textureFilename)
{
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//���������ظ������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load texture from file
	int w, h, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(textureFilename.c_str(), &w, &h, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "ERROR:: load texture failed from <- " << textureFilename << endl;
	}
	stbi_image_free(data);

	return texture;
}

void translateTest()
{
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 1.0f));

	vec = trans * vec;
	cout << "Vector(1,0,0) move to: (" << vec.x << "," << vec.y << "," << vec.z << ")" << endl;
}

void genTransform(glm::mat4& trans)
{
	glm::mat4 mat = glm::mat4(1.0f);
	// ʵ�ʵı任˳��Ӧ�����Ķ�˳���෴, Ҳ����˵����ľ������Ĳ���ʵ�����ǣ�
	// �����ţ�����ת
	mat = glm::rotate(mat, glm::radians(90.0f), glm::vec3(0, 0, 1));
	mat = glm::scale(mat, glm::vec3(0.5, 0.5, 0.5));
	trans = mat;
}

void updateTransform(glm::mat4& trans)
{
	glm::mat4 mat = glm::mat4(1.0f);
	// ����ת�����ƶ�
	mat = glm::translate(mat, glm::vec3(0.5f, -0.5f, 0.0f));
	mat = glm::rotate(mat, (float)glfwGetTime(), glm::vec3(0, 0, 1));
	trans = mat;
}

void getMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, const unsigned screenWidth, const unsigned screenHeight)
{
	// ģ�Ͷ���ӱ��ؾֲ�����任���ü�����(��OpenGLϣ����(-1, 1)�ռ䷶Χ)�Ĺ��̣�
	// local vector (vertex position) * model matrix * view matrix * projection matrix
	// ��������任��clip (-1,1)��OpenGL vertex shader ���Զ�����͸�ӳ�����ü����Ӷ������Ǳ任����׼���豸����(Normalized Device Coordinate, NDC)
	// ֮��OpenGL��ʹ��glViewPort�ڲ��Ĳ���������׼���豸����ӳ�䵽��Ļ���꣬ÿ�����궼������һ����Ļ�ϵĵ㣨�����ǵ���������һ��800x600����Ļ����������̳�Ϊ�ӿڱ任��
	// ����������˴�ģ�Ͷ��㵽��Ļ���ص�ӳ�䣡
	// 
	// ע�⣺���¹�ʽд��˳���������������෴�ģ���Ϊ��������Ĺ�ϵ
	// V_clip = M_projection * M_view * M_model * V_local
	
	// model �������ڽ�ģ�Ϳռ�����任������ռ�
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(1, 0, 0));
	// �������꣬Z��ָ����Ļ�⣬��������-Z�ƶ����൱������˺�
	view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0, 0, -3.0f));
	// ͸��ͶӰ����, ���������
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
}

void updateMVP4Shader(Shader& myShader, glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
	unsigned modelLocation = glGetUniformLocation(myShader.ID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	unsigned viewLocation = glGetUniformLocation(myShader.ID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	unsigned projectionLocation = glGetUniformLocation(myShader.ID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

glm::mat4 getAnimateView(const float radius /*= 10.0f*/)
{
	float camX = (float)sin(glfwGetTime()) * radius;
	float camZ = (float)cos(glfwGetTime()) * radius;
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(camX, 0, camZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	return view;
}

glm::mat4 handleCameraView(GLFWwindow* win, glm::vec3& cameraPos, glm::vec3& cameraFront, const glm::vec3& cameraUp /*= glm::vec3(0.0f, 1.0f, 0.0f)*/, const float speed /*= 0.05f*/)
{
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += speed * cameraFront;
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= speed * cameraFront;
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos += speed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos -= speed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return view;
}

void getDeltaTime(float& deltaTime, float& lastTime)
{
	float currentTime = (float)glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}