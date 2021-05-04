#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;


void main()
{
   
	FragColor = texture(texture1,TexCoord);
	FragColor = vec4(0.85*FragColor.r,0.85*FragColor.g,0.85*FragColor.b,FragColor.a);
}