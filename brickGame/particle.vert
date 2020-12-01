#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 Projection;
uniform vec2 offset;
uniform vec4 color;

void main()
{
	float Scale = 10.0f;
	TexCoords = vertex.zw;
	ParticleColor = color;
	gl_Position = Projection*vec4(vertex.xy*Scale+offset,0.0,1.0);
}