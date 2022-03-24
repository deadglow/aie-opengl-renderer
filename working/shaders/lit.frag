#version 460

#define LIT
#define FOG
#define LIGHTS_MAX 5

// structs
struct DirectionLight
{
	vec3 direction;
	vec4 color;
};

struct PointLight
{
	vec3 position;
	vec4 color;
	vec4 properties;
};

// uniform buffers
layout (std140, binding = 0) uniform _Camera
{
						// size		//offset
	mat4 _Pmat;			// 64		// 0
	mat4 _iPmat;		// 64		// 64
	mat4 _Vmat;			// 64		// 128
	mat4 _iVmat;		// 64		// 192
};

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
};

layout(std140, binding = 2) uniform _Lighting
{
								// size		// offset
	DirectionLight _DirLight;	// 16		0
	vec4 _Ambient;				// 16		32
};

layout(std140, binding = 3) uniform _PointLightData
{
	// size: 48 * LIGHTS_MAX
	PointLight _PointLights[LIGHTS_MAX];
};

layout(std140, binding = 4) uniform _Fog
{
								// size		// offset
	vec4 _FogColor;				// 16		// 0
	float _FogDensity;			// 4		// 16
};


// uniforms //

// model data
uniform mat4 _M2W;
uniform mat4 _iM2W;

// sampler
layout (binding = 0) uniform sampler2D _Texture0;		// default
layout (binding = 1) uniform sampler2D _Texture1;		// diffuse
layout (binding = 2) uniform sampler2D _Texture2;		// normal map
layout (binding = 3) uniform sampler2D _Texture3;		// specular map
uniform vec4 _AlbedoColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform float _NormalMapScale = 1.0;

// specular
uniform float _SpecularStrength = 1.5;
uniform float _Smoothness = 0.9;

//inout
in VS_OUT
{
	in vec3 FragPos;
	in mat3 TBN;
	in vec2 TexCoord;
} fs_in;

out vec4 FragColour;

// functions
float CalculateSpecularIntensity(vec3 norm, vec3 lightDir, vec3 fragDir, float radius)
{
	// specular highlight
	float t = _Smoothness * texture(_Texture3, fs_in.TexCoord).r;
	float exponent = mix(2, 1024, _Smoothness);
	vec3 halfwayDir = normalize(lightDir + fragDir);
	float spec = pow(max(dot(-halfwayDir, norm), 0.0), exponent);

	return _SpecularStrength * spec;
}

vec4 CalculateDirectionalLight(vec3 norm, vec3 fragDir)
{
	vec4 dir4 = _Vmat * vec4(_DirLight.direction, 0.0);
	vec3 dir = dir4.xyz;
	// light
	float diffuse = max(dot(norm, -dir), 0.0);

	// specular highlight
	float specular = CalculateSpecularIntensity(norm, dir, fragDir, 0.0);

	return (diffuse + specular) * _DirLight.color;
}

vec4 CalculatePointLights(vec3 norm, vec3 fragDir)
{
	vec4 lightTotal;
	for (int i = 0; i < LIGHTS_MAX; i++)
	{
		// light vec
		vec3 lightPos = (_Vmat * vec4(_PointLights[i].position, 1.0)).xyz;
		
		vec3 lightDir = fs_in.FragPos - lightPos;
		float dist = length(lightDir);
		lightDir = lightDir / dist;

		float diffuse = max(dot(norm, -lightDir), 0.0);

		// specular
		float specular = CalculateSpecularIntensity(norm, lightDir, fragDir, _PointLights[i].properties.w);

		// attenuation
		float attenuation = 1.0 / (_PointLights[i].properties.x + _PointLights[i].properties.y * dist + _PointLights[i].properties.z * (dist * dist));

		lightTotal += (diffuse + specular) * _PointLights[i].color * attenuation;
	}
	return lightTotal;
}

vec4 ProcessFog(vec4 color)
{
	vec3 delta = -fs_in.FragPos;
	float depth = dot(delta, delta) * _FogDensity;
	float t = clamp(depth, 0, 1);

	return mix(color, _FogColor, t);
}

void main()
{
	vec3 norm = texture(_Texture2, fs_in.TexCoord).rgb;
	norm = norm * 2.0 - 1.0;
	norm = normalize(fs_in.TBN * norm);

	vec3 viewDir = vec3(0, 0, 1);
	vec3 fragDir = normalize(fs_in.FragPos);

	vec4 totalLight;
	totalLight += CalculateDirectionalLight(norm, fragDir);
	totalLight += CalculatePointLights(norm, fragDir);
	totalLight += _Ambient;

	vec4 color = texture(_Texture1, fs_in.TexCoord) * _AlbedoColor * totalLight;
	
	// do fog
	FragColour = ProcessFog(color);
}