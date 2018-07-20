//$Version$ 
#version 330 core

//$VaryingsIn$

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangents;
layout (location = 4) in vec3 aBitangents;

// -$

//$Varyings$

// out <type> VariableName ;


//out vec3 vFragPos;
//out vec2 vTexCoords;


// -$

//Uniform Buffer Object - Be mindful of the memory layout for different hardware maybe
layout (std140) uniform Matrices
{
	uniform mat4 View;
	uniform mat4 Projection;
	
};

//Uniforms-Standard
uniform mat4 Model;
uniform mat4 MVP ;
uniform float test;
uniform mat4 view;
uniform vec4 InputCol;


//$Uniforms$



// -$


//$Constants$

// -$



void main()
{


	//Main - Default
	//vTexCoords= aTexCoords;
    gl_Position = (Projection* View* Model* vec4(aPos, 1.0)) ; 
	//$MainGeom$

	// -$

	//$Main$
	

	// -$

}




