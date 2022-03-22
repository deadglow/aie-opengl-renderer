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

// base data
uniform float _Time;
uniform mat4 _M2W;
uniform mat4 _iM2W;
uniform mat4 _normalMat;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
	// local to view
	FragPos = vec3(_Vmat * _M2W * vec4(_Position, 1));
	// world to perspective
	gl_Position = _Pmat * vec4(FragPos, 1.0);
	Normal = mat3(_normalMat) * _Normal;
	TexCoord = _TexCoord;
}