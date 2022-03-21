#version 450

in vec3 Normal;
in vec2 TexCoord;
in vec4 NDCPos;
in vec3 FragPos;
out vec4 FragColour;

uniform sampler2D _Texture;
uniform vec4 _AlbedoColor = vec4(1.0, 1.0, 1.0, 1.0);

uniform float _Time;
uniform mat4 _VP;
uniform mat4 _iVP;
uniform mat4 _M2W;
uniform mat4 _NormalMatrix;
uniform vec3 _CamPos;
uniform vec3 _CamDir;
uniform float _NearZ;
uniform float _FarZ;
uniform vec4 _Ambient = vec4(0.1, 0.1, 0.1, 1) * 0.5;

uniform vec3 _DirLight = vec3(0, -1, 0);
uniform vec4 _LightCol = vec4(1.5, 1.5, 1.5, 1.5);
uniform float _SpecularStrength = 1.0;
uniform float _Roughness = 0.3;

uniform vec4 _FogColor = vec4(0.0f, 0.02f, 0.07f, 1.0f);
uniform float _FogDensity = 0.0015;

void main()
{
	// fog
	vec4 worldPos = _iVP * NDCPos;
	vec3 delta = _CamPos - worldPos.xyz;
	float depth = dot(delta, delta) * _FogDensity;
	float t = clamp(depth, 0, 1);

	FragColour = mix(texture(_Texture, TexCoord) * _AlbedoColor, _FogColor, t);
}