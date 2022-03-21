#version 450

in vec3 Normal;
in vec2 TexCoord;
in vec4 Color;
in vec4 NDCPos;
out vec4 FragColour;

uniform sampler2D _Texture;

uniform float _Time;
uniform mat4 _MVP;
uniform mat4 _iVP;
uniform vec3 _CamPos;
uniform vec3 _CamDir;
uniform float _NearZ;
uniform float _FarZ;

uniform vec4 _FogColor = vec4(0, 0, 0, 1);
uniform float _FogDensity = 0.003;

void main()
{
	vec4 worldPos = _iVP * NDCPos;
	vec3 delta = _CamPos - worldPos.xyz;
	float depth = dot(delta, delta) * _FogDensity;
	float t = clamp(depth, 0, 1);
	FragColour = mix(texture(_Texture, TexCoord) * Color, _FogColor, t);
}