#version 330 core
layout( location=0 ) out vec4 FragColor;

//in vec3 fColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec4 InputCol;
uniform float test;

uniform mat4 view;







void main()
{
	
    
	float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * InputCol.xyz;

	vec3 result = ambient * vec3(1.0,1.0,1.0);
	
	FragColor = vec4(result, 1.00000);

	//------------------
	
	/*const vec3 lightPos = vec3(1.0,1.0,1.0);
	const vec3 ambientColor = vec3(0.1, 0.0, 0.0);
	const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
	const vec3 specColor = vec3(1.0, 1.0, 1.0);
	*/

	
  

  //----------------------------



	

	// $
	



}