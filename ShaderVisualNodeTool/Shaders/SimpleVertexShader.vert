//$Version$ 
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangents;
layout (location = 4) in vec3 aBitangents;

//$Varyings$


//Uniforms-Standard
layout (std140) uniform Matrices
{
	uniform mat4 View;
	uniform mat4 Projection;
	
};
uniform mat4 Model;
uniform mat4 MV;
uniform mat4 MVP ;

//$Uniforms$


//$Constants$


void main()
{

	//Main - Default
    gl_Position = (Projection* View* Model* vec4(aPos, 1.0)) ; 


	//$Main$
	

	//$MainLoop$
	// -$


}




