#version 450

//Attributes that we expect from the vertex buffer
layout (location = 0) in vec3 _Position;
layout (location = 1) in vec4 _VertexColor;

uniform float _Time;

out vec4 Color;

void main()
{
	gl_Position = vec4(_Position, 1);
	Color = _VertexColor;
}