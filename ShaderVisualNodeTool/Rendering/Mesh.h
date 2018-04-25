#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"



struct Vertex {

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
};

struct Texture {
	int ID;
	std::string type;
	std::string path;

};

class Mesh
{

private:

	unsigned int  VBO, EBO;

	//Setup Mesh -- OpenGL only for now
	void SetupMesh();
public:
	//Mesh data 
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture> Textures;
	unsigned int VAO;

	//constructors
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	//copy constructor
	Mesh(const Mesh &other);
	//destructor
	~Mesh();

	//Draw of the mesh
	void Draw(Shader shader);

};

