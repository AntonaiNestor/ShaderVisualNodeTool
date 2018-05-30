#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//passed to the fragment shader for interpolation
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;


//Uniform Buffer Object - Be mindful of the memory layout for different hardware maybe
layout (std140) uniform Matrices
{
	uniform mat4 View;
	uniform mat4 Projection;
	
};

uniform mat4 Model;


void main()
{
	//Normal=aNormal;

    TexCoord=aTexCoords;

	//view position 
	

	//normal passing
	mat3 normalMatrix = transpose(inverse(mat3(View * Model)));
    Normal = normalize(normalMatrix * aNormal);

	//the current vertex position in view space
	
	vec4 vertPos4 = View * Model * vec4(aPos, 1.0);
    FragPos = vec3(vertPos4) / vertPos4.w;

	//gl position
    gl_Position = Projection * View * Model* vec4(aPos, 1.0); 



}




