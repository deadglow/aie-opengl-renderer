#version 460

//Attributes that we expect from the vertex buffer
layout (location = 0) in vec3 _Position;
layout (location = 1) in vec3 _Normal;
layout (location = 2) in vec3 _Tangent;
layout (location = 3) in vec2 _TexCoord;

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
};

// out
out VS_OUT
{
	out vec2 TexCoord;
} vs_out;

void main()
{
	// world to perspective
	gl_Position = vec4(_Position, 1.0);
	
	vs_out.TexCoord = _TexCoord;
}