
layout( location=0 ) out vec4 FragColor;

//in vec3 fColor;
uniform vec4 InputCol;
uniform float test;

void main()
{
	
    //FragColor = vec4(1.0,0.0,0.0,1.0);   
	float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * InputCol.xyz;

    vec3 result = ambient * vec3(1.0,1.0,1.0);
    FragColor = vec4(result, 1.0);
}