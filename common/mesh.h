#pragma once

#include <vector>
#include "shaderClass.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
};

struct Texture
{
	unsigned int id;
	string type;
	string path;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	/// <summary>
	/// ע��shader����ͼ�����淶������������ʽ: tex_{textureType}{N}
	/// ����textureTypeΪdiffuse��specular, NΪ����1,2,3...
	/// ����: uniform sampler2D tex_diffuse1; uniform sampler2D tex_specular1
	/// </summary>
	/// <param name="shader"></param>
	void Draw(Shader shader);
protected:
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};