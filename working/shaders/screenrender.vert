#version 460

//Attributes that we expect from the vertex buffer
layout (location = 0) in vec3 _Position;
layout (location = 1) in vec3 _Normal;
layout (location = 2) in vec3 _Tangent;
layout (location = 3) in vec2 _TexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(_Position, 1.0);
	
	TexCoord = _TexCoord;
}