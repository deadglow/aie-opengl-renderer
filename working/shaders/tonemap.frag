#version 460

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
	float _Aspect;
};

uniform float _Exposure = 2;
// sampler
layout (binding = 0) uniform sampler2D _MainTexture;
layout (binding = 2) uniform sampler2D _PostprocessingTexture;

in vec2 TexCoord;
out vec4 FragColour;


// functions
void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(_MainTexture, TexCoord).rgb;

	// exposure tone mapping
	vec3 mapped = vec3(1.0) - exp(-hdrColor * _Exposure);

	mapped = pow(mapped, vec3(1.0 / gamma));

	FragColour = vec4(mapped, 1.0);	
}