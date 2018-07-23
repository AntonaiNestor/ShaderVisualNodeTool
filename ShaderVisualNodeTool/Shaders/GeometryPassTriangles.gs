//$Version$ 
#version 330 core


//ASK MORTEN ABOUT THIS
layout (triangles) in;
//layout (points, max_vertices = 256) out;

//EXTREMELY IMPORTANT :: 
// When I want to pass a per vertex varying from vertex to fragment 
// Do it Like BELOW:
// VERTEX -> 1 value
// array of values depending on prim-> GEOMETRY -> 1 value which changes in between emit vertex
// 1 value -> FRAGMENT

//$Varyings$

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

int NumOutVertices = 3;
// -$

void main() {    

	

	//$Main$
	
	//Default Pass through shader 
	//Per vertex section

	//Not a very nice way to limit the numoutvertices but is okey for now
	if (NumOutVertices > 6){ NumOutVertices = 6;}

	//the 3 needs to change according to the primitive list
	for (int i = 0; i < NumOutVertices ; i ++)
	{
		
		 gl_Position = gl_in[i].gl_Position;
		
		//$MainGeom$
	
		// -$

		EmitVertex();
	}

    EndPrimitive();
}  