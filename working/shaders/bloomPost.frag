#version 460

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
};

// sampler
layout (binding = 0) uniform sampler2D _MainTexture;
layout (binding = 1) uniform sampler2D _BrightTexture;
layout (binding = 1) uniform sampler2D _PostprocessTexture;

uniform float _Exposure = 1.0;

in vec2 TexCoord;
out vec4 FragColour;

// functions
void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(_MainTexture, TexCoord).rgb;
	vec3 bloomColor = texOffset(_PostprocessTexture, TexCoord).rgb;
	hdrColor += bloomColor;

	// tone mapping
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	result = pow(result, vec3(1.0 / gamma));

	FragColour = vec4(result, 1.0);
}