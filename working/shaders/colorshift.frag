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
	vec4 color = texture(_PostprocessingTexture, TexCoord);

	float average = color.x + color.y + color.z;
	average /= 3;

	color = color.zyxw;

	FragColour = color;
}