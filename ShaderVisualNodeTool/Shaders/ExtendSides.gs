#version 330 core
layout (triangles) in;
layout (triangles, max_vertices = 3) out;



int uNum=55;

void main() {    
     fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex

	float dt=1./float(uNum);
	float t=0.;

	for (int i = 0; i <= uNum ; i++,t+=dt){
		float omt=1.-t;
		float omt2 =omt * omt;
		float omt3 = omt *omt2 ; 
		float t2 =t*t;
		float t3= t*t2;

		vec4 xyzw = omt3 * gl_in[0].gl_Position + 
		3. *t * omt2 * gl_in[1].gl_Position + 
		3. * t2 *omt * gl_in[2].gl_Position + 
		t3 * gl_in[3].gl_Position;

		gl_Position =xyzw;
		EmitVertex();
	}
}	