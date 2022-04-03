#version 460

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
};

// sampler
layout (binding = 0) uniform sampler2D _ScreenTexture;

//inout
in VS_OUT
{
	in vec2 TexCoord;
} fs_in;

out vec4 FragColour;

// functions
void main()
{
	float coord = max(abs(fs_in.TexCoord.x - 0.5), abs(fs_in.TexCoord.y - 0.5));
	float mult = 1 - (coord / 0.5);
	vec2 dir = vec2(cos(_Time), sin(_Time)) * mult * 0.005;
	vec4 color = texture(_ScreenTexture, fs_in.TexCoord);
	color = texture(_ScreenTexture, fs_in.TexCoord - dir) + texture(_ScreenTexture, fs_in.TexCoord + dir);
	color /= 2;
	FragColour = color;
}