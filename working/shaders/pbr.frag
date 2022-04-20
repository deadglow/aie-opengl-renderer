#version 460

#define LIGHTS_MAX 5

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

in VS_OUT
{
	in vec3 Position;
	in mat3 TBN;
	in vec2 TexCoord;
} fs_in;

layout (location = 0) out vec4 FragColour;



void main()
{
   	vec3 norm = texture(_NormalMap, fs_in.TexCoord).rgb;
	norm = norm * 2.0 - 1.0;
	norm = normalize(fs_in.TBN * norm);

   	vec3 camDir = vec3(0, 0, 1);
	// direction TO fragment
	vec3 camToFrag = normalize(fs_in.Position);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// reflectance
	vec3 Lo = vec3(0.0);
	for (int i = 0; i < _NumLights; ++i)
	{
		vec3 viewSpaceLightPos = _Vmat * _Lights[i].position;
		vec3 L = normalize(fs_in.Position - viewSpaceLightPos);
		vec3 H = normalize(camToFrag + L);

		vec3 radiance = 

		float distance 
	}
}

vec3 GetRadianceFromLight(vec3 norm, vec3 fragDir, int index)
{
	switch(_Lights[index].type)
	{
		case 0:
			return CalculateDirectionalLight(index, norm, fragDir);
		break;
		case 1:
			return CalculatePointLight(index, norm, fragDir);
		break;
	}
}

vec3 CalculateDirectionalLight(int i, vec3 norm, vec3 fragDir)
{
	vec4 dir4 = _Vmat * vec4(_Lights[i].direction, 0.0);
	vec3 dir = dir4.xyz;
	// light
	float diffuse = max(dot(norm, -dir), 0.0);

	// specular highlight
	float specular = CalculateSpecularIntensity(norm, dir, fragDir, 0.0);

	return (diffuse + specular) * _Lights[i].color.rgb;
}

vec3 CalculatePointLight(int i, vec3 norm, vec3 fragDir)
{
	// light vec
	vec3 lightPos = (_Vmat * vec4(_Lights[i].position, 1.0)).xyz;
	
	vec3 lightDir = fs_in.FragPos - lightPos;
	float dist = length(lightDir);
	lightDir = lightDir / dist;

	float diffuse = max(dot(norm, -lightDir), 0.0);

	vec4 properties = _Lights[i].properties;
	// specular
	float specular = CalculateSpecularIntensity(norm, lightDir, fragDir, properties.w);

	// attenuation
	float attenuation = 1.0 / (properties.x + properties.y * dist + properties.z * (dist * dist));

	return (diffuse + specular) * _Lights[i].color.rgb * attenuation;
}