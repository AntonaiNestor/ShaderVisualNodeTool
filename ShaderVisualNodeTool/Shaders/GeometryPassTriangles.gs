//$Version$ 
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 256) out;



//EXTREMELY IMPORTANT :: 
// When I want to pass a per vertex varying from vertex to fragment 
// Do it Like BELOW:
// VERTEX -> 1 value
// array of values depending on prim-> GEOMETRY -> 1 value which changes in between emit vertex
// 1 value -> FRAGMENT

//$Varyings$


//in VS_OUT {
// in vec3 vFragPos[];
//} gs_in[];

//out GS_OUT {
// out  vec3 gFragPos;
//} gs_out;

// -$
//in vec3 FragPos;


//Uniforms-Standard
uniform mat4 Model;
uniform mat4 MVP ;
uniform float test;
uniform mat4 view;
uniform vec4 InputCol;
uniform float time;
//$Uniforms$


// -$



//$Constants$

// -$


//Explosion shader
vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

//Explosion shader
vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 0.5;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
} 


void main() {    


//Explosion shader
//	vec3 normal = GetNormal();

 //   gl_Position = explode(gl_in[0].gl_Position, normal);
//	gs_out.gFragPos = gs_in[0].FragPos;
  //  TexCoords = gs_in[0].texCoords;
//    EmitVertex();
 //   gl_Position = explode(gl_in[1].gl_Position, normal);
//	gs_out.gFragPos = gs_in[1].FragPos;
 //   TexCoords = gs_in[1].texCoords;
 //   EmitVertex();
 //   gl_Position = explode(gl_in[2].gl_Position, normal);
//	gs_out.gFragPos = gs_in[2].FragPos;
 //   TexCoords = gs_in[2].texCoords;
//    EmitVertex();
//    EndPrimitive();
	


	//$Main$
	

	// -$


	//Default Pass through shader 

	//Per vertex section

	//the 3 needs to change according to the primitive list
	for (int i = 0; i < 3 ; i ++)
	{
		//gFragPos = vFragPos[i]; 
	    gl_Position = gl_in[i].gl_Position;

		
		//$MainGeom$
	

		// -$


		EmitVertex();
	}

    EndPrimitive();
}  