#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 Projection;

void main()
{
	TexCoords = vertex.zw;
	gl_Position = Projection*Model*vec4(vertex.xy,0.0,1.0);
}