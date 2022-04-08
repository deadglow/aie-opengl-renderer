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

uniform bool _UseMain = false;
uniform bool _Horizontal = false;
uniform float _Weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);	

in vec2 TexCoord;
out vec4 FragColour;

// functions
void main()
{
	vec2 texOffset = 1.0 / textureSize(_BrightTexture, 0);
	vec3 result = texture(image, TexCoord).rgb * _Weights[0];
	sampler2D image = (_UseMain)? _BrightTexture : _PostprocessTexture;

	if (_Horizontal)
	{
		for (int i = 1; i < 5; ++i)
		{
			result += texture(image, TexCoord + vec2(texOffset.x * i, 0.0)).rgb * _Weights[i];
			result += texture(image, TexCoord - vec2(texOffset.x * i, 0.0)).rgb * _Weights[i];
		}
	}
	else
	{
		for (int i = 1; i < 5; ++i)
		{
			result += texture(image, TexCoord + vec2(0.0, texOffset.x * i)).rgb * _Weights[i];
			result += texture(image, TexCoord - vec2(0.0, texOffset.x * i)).rgb * _Weights[i];	
		}
	}

	FragColour = vec4(result, 1.0);
}