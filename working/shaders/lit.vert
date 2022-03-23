#version 450

//Attributes that we expect from the vertex buffer
layout (location = 0) in vec3 _Position;
layout (location = 1) in vec3 _Normal;
layout (location = 2) in vec2 _TexCoord;

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

// model data
uniform mat4 _M2W;
uniform mat4 _iM2W;
uniform mat4 _NormalMat;

// out
out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
	float angle = sin(atan(_Position.x / _Position.z));
	float timeAngle = sin(radians(_Time * 30));

	float t = abs(angle - timeAngle);
	// local to view
	vec3 pos = _Position;
	pos = pos * t;
	pos.y = _Position.y;
	FragPos = vec3(_Vmat * _M2W * vec4(_Position, 1));
	// world to perspective
	gl_Position = _Pmat * vec4(FragPos, 1.0);
	Normal = vec3(_NormalMat * vec4(_Normal, 0.0));
	TexCoord = _TexCoord;
}