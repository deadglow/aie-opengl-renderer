#version 460

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
};

// sampler
layout (binding = 0) uniform sampler2D _MainTexture;
layout (binding = 2) uniform sampler2D _PostprocessingTexture;

in vec2 TexCoord;
out vec4 FragColour;

// functions
void main()
{
	vec4 color = texture(_PostprocessingTexture, TexCoord);

	float average = color.r + color.g + color.b;
	average /= 3;

	color = vec4(average, color.g, average, 1.0);

	FragColour = color;
}