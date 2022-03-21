#version 450

#define LIGHTS_MAX 5

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

uniform vec4 _FogColor = vec4(0.0f, 0.02f, 0.07f, 1.0f);
uniform float _FogDensity = 0.0015;

uniform float _SpecularStrength = 1.0;
uniform float _Roughness = 0.3;

// lights
uniform vec3 _DirLight = vec3(0, -1, 0);
uniform vec4 _DirLightColor = vec4(1.5, 1.5, 1.5, 1.5);

struct PointLight
{
	vec3 position;
	vec4 color;
}
uniform vec3 _LightPositions[LIGHTS_MAX];
uniform vec4 _LightColors[LIGHTS_MAX];

float CalculateSpecularIntensity(vec3 norm, vec3 viewDir, vec3 lightDir)
{
	// specular highlight
	float exponent = mix(2, 256, _Roughness);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), exponent);
	return _SpecularStrength * spec;

}

vec4 CalculateDirectionalLight(vec3 norm, vec3 viewDir)
{
	// light
	vec4 diffuse = max(dot(norm, -_DirLight), 0.0) * _DirLightColor;

	// specular highlight
	vec4 specular = CalculateSpecularIntensity(norm, viewDir, _DirLight) * _DirLightColor;

	return diffuse + specular;
}

vec4 CalculatePointLights(vec3 norm, vec3 viewDir)
{
	for (int i = 0; i < LIGHTS_MAX; i++)
	{
		vec3 vec = (FragPos - _LightPositions[i];
		float distance = length(vec);
		vec = vec / distance;

		float falloffValue = 1 / (distance * distance);

		vec4 diffuse = max(dot(norm, -vec), 0.0) * _LightColors[i];

		// specular
		vec4 specular = CalculateSpecularIntensity(norm, viewDIr, vec);

		return (diffuse + specular);
	}
}


void main()
{
	// fog
	vec4 worldPos = _iVP * NDCPos;
	vec3 delta = _CamPos - worldPos.xyz;
	float depth = dot(delta, delta) * _FogDensity;
	float t = clamp(depth, 0, 1);

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(_CamPos - FragPos);

	vec4 totalLight;
	totalLight += CalculateDirectionalLight(norm, viewDir);
	totalLight += CalculatePointLights(norm, viewDir);
	totalLight += _Ambient;

	FragColour = mix(texture(_Texture, TexCoord) * _AlbedoColor * totalLight, _FogColor, t);
}