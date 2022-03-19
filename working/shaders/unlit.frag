#version 450

in vec4 Color;
in vec2 TexCoord;
out vec4 FragColour;

//uniform goes here
uniform float _Time;
uniform sampler2D _Texture;

void main()
{
	FragColour = texture(_Texture, TexCoord);
}