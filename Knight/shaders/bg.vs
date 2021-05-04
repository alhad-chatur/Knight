#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 viewtranslate;
uniform mat4 viewscale;
uniform mat4 viewrotate;
uniform mat4 transform;

void main()
{
	mat4 view = viewtranslate*viewscale*viewrotate;
	vec4 pos = transform*view*model*vec4(aPos,1.0);

	gl_Position = pos;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}