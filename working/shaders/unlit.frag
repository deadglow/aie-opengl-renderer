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

layout(std140, binding = 3) uniform _Fog
{
								// size		// offset
	vec4 _FogColor;				// 16		// 0
	float _FogDensity;			// 4		// 16
};


// uniforms //

// model data
uniform mat4 _M2W;
uniform mat4 _iM2W;

// sampler
layout (binding = 0) uniform sampler2D _Texture0;		// diffuse
uniform vec4 _AlbedoColor = vec4(1.0, 1.0, 1.0, 1.0);

//inout
in VS_OUT
{
	in vec3 FragPos;
	in vec2 TexCoord;
} fs_in;

out vec4 FragColour;

// functions

vec4 ProcessFog(vec4 color)
{
	vec3 delta = -fs_in.FragPos;
	float depth = dot(delta, delta) * _FogDensity;
	float t = clamp(depth, 0, 1);

	return mix(color, _FogColor, t);
}

void main()
{
	// do fog
	FragColour = ProcessFog(texture(_Texture0, fs_in.TexCoord));
}