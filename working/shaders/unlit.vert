#version 450

//Attributes that we expect from the vertex buffer
layout (location = 0) in vec3 _Position;
layout (location = 1) in vec3 _Normal;
layout (location = 2) in vec2 _TexCoord;
layout (location = 3) in vec4 _VertexColor;

uniform float _Time;
uniform mat4 _MVP;
uniform mat4 _iVP;
uniform vec3 _CamPos;
uniform vec3 _CamDir;
uniform float _NearZ;
uniform float _FarZ;

out vec3 Normal;
out vec2 TexCoord;
out vec4 Color;
out vec4 NDCPos;

void main()
{
	NDCPos = _MVP * vec4(_Position, 1);
	gl_Position = NDCPos;
	Normal = _Normal;
	TexCoord = _TexCoord;
	Color = _VertexColor;
}