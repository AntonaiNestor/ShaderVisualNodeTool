#pragma once
#ifndef SHADER_H
#define SHADER_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>



/*
Initial Author: Joey De Vries :: add twitter etc
Modifications and this version author : Antony Nestoridis



Shader class : Currently represents a shader program consisted of all possible types
of shaders.
*/

enum ShaderType {
	VERTEX,
	TESSELATION_CONTROL,
	TESSELATION_EVALUATION,
	GEOMETRY,
	FRAGMENT
};

class Shader
{
public:

	// Program ID 
	unsigned int ID;
	// Array containing the shader objects currently contained in the program
	// -1 indicates that there is no shader available in the program yet.
	int ShaderObjIDs[5];

	std::string fragCode;
	std::string vertCode;

	// Constructors -- For now all shaders are loaded from files.
	// This will probably change when we decide to create shaders using abstract
	// syntax.
	// ------------------------------------------------------------------------

	//Default : Vertex + Fragment
	Shader(const char* vertexPath, const char* fragmentPath);

	//Vertex + geometry + Fragment
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

	// Vertex + Tcs + Tes + Geometry + Fragment
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* tcsPath, const char* tesPath);



	// activate the shader
	// ------------------------------------------------------------------------
	void Use();

	// utility uniform functions
	// ------------------------------------------------------------------------
	void SetBool(const std::string &name, bool value) const;
	// ------------------------------------------------------------------------
	void SetInt(const std::string &name, int value) const;
	//----------------------------------------------------------------------
	void SetFloat(const std::string &name, float value) const;
	// ------------------------------------------------------------------------
	void SetMat4(const std::string &name, glm::mat4 matrix) const;

	void ChangeShaders(const char* vertexPath, const char* fragmentPath);

	//this has a different functionality than the initial suggested one
	//needs to change into more generic parameters with a list
	void EditShader(std::string newVertex , std::string newFragment);

	void AddToProgram(const char* shaderPath, ShaderType type);

private:
	// utility function for checking shader compilation/linking errors.
	//  TODO: This function should  send error in debugger class.
	// ------------------------------------------------------------------------
	void CheckCompileErrors(unsigned int shader, std::string type);
};
#endif