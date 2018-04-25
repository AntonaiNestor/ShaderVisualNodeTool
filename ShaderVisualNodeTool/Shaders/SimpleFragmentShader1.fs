
out vec4 FragColor;

in vec2 TexCoord;


//uniform vec4 InputCol;
//uniform sampler2D texture_diffuse1;


void main()
{
	
    FragColor = vec4(1.0,1.0,0.0,1.0);  
	//float ambientStrength = 0.5;
    //vec3 ambient = ambientStrength * InputCol.xyz;

    //vec3 result = ambient * vec3(1.0,1.0,1.0);
    //FragColor = vec4(result, 1.0);
	//FragColor = InputCol;
	//FragColor = texture(texture_diffuse1, TexCoord);
}