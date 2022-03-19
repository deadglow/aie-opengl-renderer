#version 450

out vec4 FragColour;
in vec3 Normal;
in vec2 TexCoord;
in vec4 Color;

//uniform goes here
uniform float _Time;
uniform sampler2D _Texture;

void main()
{
	FragColour = texture(_Texture, TexCoord) * Color;
}