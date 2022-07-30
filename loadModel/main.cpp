#include "glad/glad.h"
#include "glfw3.h"

#include "../common/camera.h"
#include "../common/util.h"

#include "../common/model.h"

#include <iostream>
using namespace std;

//������α�����main���涨�壬��Ҫ����init window��callback����
const unsigned screenWidth = 800;
const unsigned screenHeight = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool isFirstMouse = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main(int argc, char* argv[])
{
	/*for (int i = 0; i < argc; i++)
	{
		cout << argv[i] << endl;
	}*/

	GLFWwindow* win;
	init(win, u8"ѧϰOpenGL_LoadModel", screenWidth, screenHeight, 3, 3);

	stbi_set_flip_vertically_on_load(true);
	//glEnable(GL_DEPTH_TEST);

	Shader modelShader("model.vert", "model.frag");
	//modelShader.use();

	// ע��: Ŀ¼���ļ���֮��ָ���������"/", ��Ϊ��directory = path.substr(0, path.find_last_of("/"));
	//string path = R"(E:\git\learnOpenGL\loadModel\nanosuit/nanosuit.obj)";
	//string path = R"(E:\fbx\Textures/NPC_6.fbx)";
	string path = R"(E:/fbx/Meshs/textureAtlas.fbx)";
	//string path = R"(E:/fbx/Meshs/textureAtlas.obj)";
	//string path = R"(E:/fbx/Meshs/cube.fbx)";
	//string path = R"(E:/fbx/Meshs/cube.obj)";
	Model theModel(path);
	theModel.ShowInfo4Debug();

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	getMVP(model, view, projection, screenWidth, screenHeight);

	while (!glfwWindowShouldClose(win))
	{
		// switch wireframe mode
		if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS)
		{
			//cout << "pressed 2" << endl;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(win);

		// rendering..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		modelShader.use();

		projection = glm::perspective(glm::radians(camera.Zoom), 1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // it's a bit too big for our scene, so scale it down
		updateMVP4Shader(modelShader, model, view, projection);

		// draw model
		theModel.Draw(modelShader);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}