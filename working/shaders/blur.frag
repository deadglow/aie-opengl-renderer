#version 460

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
};

// sampler
layout (binding = 0) uniform sampler2D _PostprocessingTexture;
layout (binding = 1) uniform sampler2D _MainTexture;

in vec2 TexCoord;
out vec4 FragColour;

// functions
void main()
{
	vec2 dir = vec2(cos(_Time), sin(_Time)) * 0.05;

	FragColour = texture(_PostprocessingTexture, TexCoord + dir);	
}