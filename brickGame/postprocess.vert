#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main()
{
	gl_Position = vec4(vertex.xy,0.0f,1.0f);
	vec2 tex = vertex.zw;
	if(chaos)
	{
		float strength = 0.3f;
		TexCoords = tex + vec2(sin(time), cos(time))*strength;
	}
	else if(confuse)
	{
		TexCoords = vec2(1.0f) - tex;
	}
	else
	{
		TexCoords = tex;
	}
	if(shake)
	{
		float strength = 0.01;
		gl_Position.x += cos(time*10)*strength;
		gl_Position.y += cos(time*15)*strength;
	}
}