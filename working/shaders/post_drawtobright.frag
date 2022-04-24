#version 460

// sampler
layout (binding = 0) uniform sampler2D _MainTexture;

uniform float _Threshold = 1.0;
uniform float _ClampValue = 300.0f;

in vec2 TexCoord;

layout (location = 0) out vec4 BrightColour;

// functions
void main()
{
	vec3 col = texture(_MainTexture, TexCoord).rgb;
	float brightness = dot(col, vec3(0.2126, 0.7152, 0.0722));

	float clampRatio = brightness / _ClampValue;

	col /= max(clampRatio, 1);

	if (brightness > _Threshold)
		BrightColour = vec4(col, 1.0);
	else
		BrightColour = vec4(0, 0, 0, 1.0);
}