#version 460

//Attributes that we expect from the vertex buffer
layout (location = 0) in vec3 _Position;

layout (std140, binding = 0) uniform _Camera
{
	mat4 _Pmat;
	mat4 _iPmat;
	mat4 _Vmat;
	mat4 _iVmat;
};

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
};


out vec3 TexCoord;

void main()
{
	// world to perspective
	TexCoord = _Position;
	gl_Position = _Pmat * _Vmat * vec4(_Position, 0.0);
}