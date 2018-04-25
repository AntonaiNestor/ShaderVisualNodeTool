#pragma once

#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>	
#include "stb_image.h"

class Model
{
public:
	
	/*  Functions   */

	//Constructor - Loads the model from filepath
	Model(char *path);
	
	//Draw Model - Draw all meshes
	void Draw(Shader shader);
private:
	/*  Model Data  */
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;
	/*  Functions   */

	void loadModel(char* path);

	//Recurcive function that traverses the assimp scene
	void processNode(aiNode *node, const aiScene *scene);

	//Converts a mesh from assimp format to our format
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	//Loads the model's textures from the assimp material
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);

	//Loads and creates a texture using stbi_image. The ID of the texture ready to be 
	// used.
	unsigned int TextureFromFile(const char *path, const std::string &directory);
	
};


