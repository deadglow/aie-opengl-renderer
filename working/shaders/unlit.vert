#version 450

//Attributes that we expect from the vertex buffer
layout (location = 0) in vec3 _Position;
layout (location = 1) in vec3 _Normal;
layout (location = 2) in vec2 _TexCoord;
layout (location = 3) in vec4 _VertexColor;

uniform float _Time;

out vec4 Color;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
	gl_Position = vec4(_Position, 1);
	Normal = _Normal;
	TexCoord = _TexCoord;
	Color = _VertexColor;
}