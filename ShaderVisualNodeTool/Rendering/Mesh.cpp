#include "Mesh.h"
#include "../NodeSystem/Graph.h"


Mesh::Mesh() {
}


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->Vertices = vertices;
	this->Indices = indices;
	SetupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->Vertices = vertices;
	this->Indices = indices;
	this->Textures = textures;
	SetupMesh();
}

Mesh::Mesh(const Mesh &other) {
	this->Vertices = other.Vertices;
	this->Indices = other.Indices;
	SetupMesh();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}

void Mesh::SetupMesh()
{
	//Generate Buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	//Buffer vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
	//Buffer indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);


	// Vertex attributes contains positions-normals-texcoords 
	//One VBO active, so the vert attributes go into its data to find the attribs.
	//position attribute in slot 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


	////normal attribute in slot 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	//texcoord attribute in slot 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void Mesh::Draw(Shader shader) {


	//the shader program used is passed to the draw
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	//for (unsigned int i = 0; i < Textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
	//									  // retrieve texture number (the N in diffuse_textureN)
	//	std::string number;
	//	std::string name = Textures[i].type;
	//	if (name == "texture_diffuse")
	//		number = std::to_string(diffuseNr++);
	//	else if (name == "texture_specular")
	//		number = std::to_string(specularNr++);
	//	//"material." +
	//	glBindTexture(GL_TEXTURE_2D, Textures[i].ID);
	//	//shader.setFloat((name + number).c_str(), i);

	//}
	//glActiveTexture(GL_TEXTURE0);

	shader.Use();
	// draw mesh
	glBindVertexArray(VAO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	glDrawElements(Graph::getInstance()->VertexTopologyOut, Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// TODO : If statements here for cases :
	//				1) No indices
	//				2) No tesselation shader so no patches
	//				3) Different draw types
	//glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, 0);
}


