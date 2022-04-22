#version 460

#define LIGHTS_MAX 5
#define M_PI 3.1415926535897932384626433832795

// structs
struct Light
{
	vec3 position;		// unused for directional
	vec3 direction;		// unused for point
	vec4 color;			// used for all
	
	vec4 properties;	// depends on light source:
						// directional: nothing
						// point: constant, linear, quadratic, radius
	int type; 			// 0 = directional, 1 = point, 2 = spot
};

struct PBRLightData
{
	vec3 color;
	vec3 lightVector;
};

// uniform buffers
layout (std140, binding = 0) uniform _Camera
{
	mat4 _Pmat;
	mat4 _iPmat;
	mat4 _Vmat;
	mat4 _iVmat;
};

layout(std140, binding = 1) uniform _Globals
{
	float _Time;
	float _DeltaTime;
	float _Aspect;
};

layout(std140, binding = 2) uniform _Lighting
{
	vec4 _Ambient;
	Light _Lights[LIGHTS_MAX];
	int _NumLights;
};

layout(std140, binding = 3) uniform _Fog
{
	vec4 _FogColor;
	float _FogDensity;
};

// model data
uniform mat4 _M2W;
uniform mat4 _iM2W;

layout (binding = 0) uniform sampler2D _Albedo;
layout (binding = 1) uniform sampler2D _NormalMap;
layout (binding = 2) uniform sampler2D _Metallic;
layout (binding = 3) uniform sampler2D _AOMap;

uniform vec3 _AlbedoColor = vec3(1.0);
uniform float _RoughnessMultiplier = 1.0;
uniform float _MetallicMultiplier = 1.0;

in VS_OUT
{
	in vec3 Position;
	in mat3 TBN;
	in vec2 TexCoord;
} fs_in;

layout (location = 0) out vec4 FragColor;

// fresnel function
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// normal distribution function
// using Trowbridge-Reitz GGX
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = M_PI * denom * denom;

	return a2 / denom;
}

// geometry function
// Schlick-GGX
float GeometrySGGX(float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;

	float denom = NdotV * (1.0 - k) + k;

	return NdotV / denom;
}

// smith geometry function
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySGGX(NdotL, roughness);
	float ggx2 = GeometrySGGX(NdotV, roughness);

	return ggx1 * ggx2;
}

PBRLightData CalculateDirectionalLight(int i)
{
	vec4 dir4 = _Vmat * vec4(_Lights[i].direction, 0.0);
	vec3 dir = dir4.xyz;

	PBRLightData data;
	data.color = _Lights[i].color.rgb;
	data.lightVector = dir;
	
	return data;
}

PBRLightData CalculatePointLight(int i)
{
	// light vec
	vec3 lightPos = (_Vmat * vec4(_Lights[i].position, 1.0)).xyz;
	
	vec3 lightDir = fs_in.Position - lightPos;
	float dist = length(lightDir);
	lightDir = lightDir / dist;

	vec4 properties = _Lights[i].properties;

	// attenuation
	float attenuation = 1.0 / (properties.x + properties.y * dist + properties.z * (dist * dist));

	PBRLightData data;
	data.color = _Lights[i].color.rgb * attenuation;
	data.lightVector = lightDir;

	return data;
}

PBRLightData GetRadianceFromLight(int index)
{
	switch(_Lights[index].type)
	{
		case 0:
			return CalculateDirectionalLight(index);
		break;
		case 1:
			return CalculatePointLight(index);
		break;
	}
}

void main()
{
   	vec3 norm = texture(_NormalMap, fs_in.TexCoord).rgb;
	norm = norm * 2.0 - 1.0;
	norm = normalize(fs_in.TBN * norm);

   	vec3 camForward = vec3(0, 0, 1);
	// direction TO fragment
	vec3 camToFrag = normalize(fs_in.Position);

	vec3 albedo = texture(_Albedo, fs_in.TexCoord).xyz * ;
	vec4 metallicSample = texture(_Metallic, fs_in.TexCoord);

	float metallic = metallicSample.r * _MetallicMultiplier;
	float roughness = metallicSample.w * _RoughnessMultiplier;
	
	vec3 ao = texture(_AOMap, fs_in.TexCoord).xyz;

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo.xyz, metallic);

	// reflectance
	vec3 Lo = vec3(0.0);
	for (int i = 0; i < _NumLights; ++i)
	{
		// this is the vector that changes based on the light (light direction)
		PBRLightData data = GetRadianceFromLight(i);
		vec3 radiance = data.color;
		vec3 L = data.lightVector;

		vec3 H = normalize(camToFrag + L);

		// base metallic F0 is 0.04, mix with albedo based on metallic
		vec3 F0 = vec3(0.04);
		F0 = mix(F0, albedo.xyz, metallic);
		vec3 F = FresnelSchlick(max(dot(H, camToFrag), 0.0), F0);
		
		float D = DistributionGGX(norm, H, roughness);
		float G = GeometrySmith(norm, camToFrag, L, roughness);

		vec3 numerator = F * D * G;
		float denom = 4.0 * max(dot(norm, camToFrag), 0.0) * max(dot(norm, L), 0.0) + 0.0001;
		vec3 specular = numerator / denom;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;

		kD *= 1.0 - metallic;

		float NdotL = max(dot(norm, L), 0.0);
		Lo += (kD * albedo / M_PI + specular) * radiance * NdotL;
	}

	vec3 ambient = _Ambient.xyz * albedo * ao;
	vec3 color = ambient + Lo;

	FragColor = vec4(color, 1.0);
}