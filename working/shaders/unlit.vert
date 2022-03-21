#version 450

//Attributes that we expect from the vertex buffer
layout (location = 0) in vec3 _Position;
layout (location = 1) in vec3 _Normal;
layout (location = 2) in vec2 _TexCoord;

uniform float _Time;
uniform mat4 _VP;
uniform mat4 _iVP;
uniform mat4 _M2W;
uniform mat4 _NormalMatrix;
uniform vec3 _CamPos;
uniform vec3 _CamDir;
uniform float _NearZ;
uniform float _FarZ;

out vec3 Normal;
out vec2 TexCoord;
out vec4 NDCPos;
out vec3 FragPos;

void main()
{
	FragPos = vec3(_M2W * vec4(_Position, 1));
	NDCPos = _VP * vec4(FragPos, 1.0);
	gl_Position = NDCPos;
	Normal = mat3(_NormalMatrix) * _Normal;
	TexCoord = _TexCoord;
}