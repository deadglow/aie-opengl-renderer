#version 460

//Attributes that we expect from the vertex buffer
layout (location = 0) in vec3 _Position;
layout (location = 1) in vec3 _Normal;
layout (location = 2) in vec3 _Tangent;
layout (location = 3) in vec2 _TexCoord;


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
out VS_OUT
{
	out vec3 FragPos;
	out mat3 TBN;
	out vec2 TexCoord;
} vs_out;

void main()
{
	// local to view
	vs_out.FragPos = vec3(_Vmat * _M2W * vec4(_Position, 1));
	// world to perspective
	gl_Position = _Pmat * vec4(vs_out.FragPos, 1.0);
	
	vec3 normal = normalize(_Normal);
	vec3 tangent = normalize(_Tangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 biTangent = cross(normal, tangent);
	
	vs_out.TBN = mat3(_NormalMat) * mat3(tangent, biTangent, normal);

	vs_out.TexCoord = _TexCoord;
}