#version 460

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
	float _Aspect;
};

// sampler
layout (binding = 0) uniform sampler2D _PostprocessingTexture;
layout (binding = 1) uniform sampler2D _MainTexture;

in vec2 TexCoord;
out vec4 FragColour;

const float BASEOFFSET = 0.002;

// functions
void main()
{
	float offset = BASEOFFSET + (sin(_Time) * 0.5 + 0.5) * 0.001;
	vec2 offsets[9] = vec2[]
	(
		vec2(-offset, offset),
		vec2(0.0, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0),
		vec2(0.0, 0.0),
		vec2(offset, 0.0),
		vec2(-offset, -offset),
		vec2(0.0, -offset),
		vec2(offset, -offset)
	);


	float kernel[9] = float[]
	(
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; ++i)
	{
		sampleTex[i] = vec3(texture(_PostprocessingTexture, TexCoord + offsets[i] * vec2(1, _Aspect)));
	}
	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; ++i)
	{
		col += sampleTex[i] * kernel[i];
	}

	FragColour = vec4(col, 1.0);	
}