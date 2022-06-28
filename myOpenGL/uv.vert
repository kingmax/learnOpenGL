#version 460 core

layout (location = 0)
in vec3 pos;
layout (location = 1)
in vec3 col;
layout (location = 2)
in vec2 uv;

out vec3 ourColor;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(pos, 1.0);
	ourColor = col;
	texCoord = uv;
}