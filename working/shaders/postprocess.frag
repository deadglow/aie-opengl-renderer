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
	FragColour = texture(_ScreenTexture, fs_in.TexCoord);
}