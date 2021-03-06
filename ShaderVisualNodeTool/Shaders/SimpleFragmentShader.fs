//$Version$ 
#version 330 core
layout( location = 0 ) out vec4 FragColor;

//in vec3 fColor;

//Varying-Standard

//$Varyings$

// -$

//Uniform Buffer Object - Be mindful of the memory layout for different hardware maybe
layout (std140) uniform Matrices
{
	uniform mat4 View;
	uniform mat4 Projection;
	
};

//Uniforms-Standard
uniform mat4 Model;
uniform mat4 MV;
uniform mat4 MVP ;
uniform float test;
uniform mat4 view;
uniform vec4 InputCol;

//$Uniforms$
//in vec2 GTex;
// -$


//$Constants$

// -$

void main()
{
	
    //Main - Default
	float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * InputCol.xyz;

	vec3 result = ambient * vec3(1.0,1.0,1.0);
	
	FragColor = InputCol;
	
	//$MainLoop$
	// -$


	//$Main$
	

	// -$
	

}