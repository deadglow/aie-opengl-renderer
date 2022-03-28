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
};

// sampler
layout (binding = 0) uniform samplerCube _Cubemap;		// cubemap
uniform vec4 _AlbedoColor = vec4(1.0, 1.0, 1.0, 1.0);

//inout
in VS_OUT
{
	in vec3 TexCoord;
} fs_in;

out vec4 FragColour;

void main()
{
	FragColour = texture(_Cubemap, TexCoord) * _AlbedoColor;
}