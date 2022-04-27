#version 460

// uniform buffers
layout (std140, binding = 0) uniform _Camera
{
						// size		//offset
	mat4 _Pmat;			// 64		// 0
	mat4 _iPmat;		// 64		// 64
	mat4 _Vmat;			// 64		// 128
	mat4 _iVmat;		// 64		// 192
};

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
	float _Aspect;
};

// sampler
layout (binding = 15) uniform samplerCube _Cubemap;		// cubemap
layout (binding = 14) uniform samplerCube _Test;
uniform vec4 _AlbedoColor = vec4(1.0, 1.0, 1.0, 1.0);

//inout
in vec3 TexCoord;

layout (location = 0) out vec4 FragColour;

void main()
{
	FragColour = texture(_Cubemap, TexCoord) * _AlbedoColor;
}