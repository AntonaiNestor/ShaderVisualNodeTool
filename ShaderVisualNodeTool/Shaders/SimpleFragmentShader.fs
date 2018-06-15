//$Version$ 
#version 330 core
layout( location = 0 ) out vec4 FragColor;

//in vec3 fColor;

//Varying-Standard
//in vec3 Normal;
//in vec3 FragPos;

//$Varyings$


// -$

//Uniforms-Standard
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
	float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * InputCol.xyz;

	vec3 result = ambient * vec3(1.0,1.0,1.0);
	
	FragColor = InputCol;

	//$Main$
	

	// -$
	

}