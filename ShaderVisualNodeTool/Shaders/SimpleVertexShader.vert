#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//passed to the fragment shader for interpolation
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;


//Uniform Buffer Object - Be mindful of the memory layout for different hardware maybe
//layout (std140, binding = 0) uniform Matrices
//{
	
	uniform mat4 View;
	uniform mat4 Projection;
//};

uniform mat4 Model;
float test1;
int cool;
vec2 FUN;

void main()
{
	Normal=aNormal;
    TexCoord=aTexCoords;
	
	//the current vertex position in view space
	FragPos= vec3(View*vec4(aPos,1.0)) ;
    gl_Position = Projection* View* Model* vec4(aPos, 1.0); 
}




