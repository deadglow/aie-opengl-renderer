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
	float _Aspect;
};


out vec3 TexCoord;

void main()
{
	// world to perspective
	vec3 tex = normalize(-_Position);
	TexCoord = tex;

	vec4 pos = _Pmat * _Vmat * vec4(_Position * 1, 0.0);
	gl_Position = pos.xyww;
}