#version 460

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
};

// sampler
layout (binding = 0) uniform sampler2D _MainTexture;
layout (binding = 1) uniform sampler2D _BrightTexture;
layout (binding = 2) uniform sampler2D _PostprocessTexture;

uniform float _Exposure = 1.0;
const float GAMMA = 2.2;

in vec2 TexCoord;
out vec4 FragColour;

vec3 Reinhard(vec3 col)
{
	vec3 result = vec3(1.0) - exp(-col);
	return result;
}

vec3 ACES(vec3 col)
{
	const float a = 2.51;
	const float b = 0.03;
	const float c = 2.43;
	const float d = 0.59;
	const float e = 0.14;
	return clamp((col * (a * col + b)) / (col * (c * col + d) + e), 0, 1);
}

// functions
void main()
{
	vec3 hdrColor = texture(_MainTexture, TexCoord).rgb;
	vec3 bloomColor = texture(_PostprocessTexture, TexCoord).rgb;
	hdrColor += bloomColor;

	// tone mapping
	vec3 result = ACES(hdrColor * _Exposure);
	result = pow(result, vec3(1.0 / GAMMA));

	FragColour = vec4(result, 1.0);
}