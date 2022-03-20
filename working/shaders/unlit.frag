#version 450

in vec3 Normal;
in vec2 TexCoord;
in vec4 Color;
out vec4 FragColour;

//uniform goes here
uniform float _Time;
uniform sampler2D _Texture;

void main()
{
	FragColour = texture(_Texture, TexCoord) * Color;
}