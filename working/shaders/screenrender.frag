#version 460

// sampler
layout (binding = 0) uniform sampler2D _MainTexture;
layout (binding = 2) uniform sampler2D _PostprocessingTexture;

uniform bool _UseMainBuffer = false;

in vec2 TexCoord;
out vec4 FragColour;

// functions
void main()
{
	if (_UseMainBuffer)
		FragColour = texture(_MainTexture, TexCoord);
	else
		FragColour = texture(_PostprocessingTexture, TexCoord);
}