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

// light memory layout
/*
					// size		// offset
vec3 position;		// 16		// 0
vec3 direction;		// 16		// 16
vec4 color;			// 16		// 32
vec4 properties;	// 16		// 48
int type 			// 4		// 64
// total size = 68 + 12(pad) = 80
*/ 

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
	float _Aspect;
};

layout(std140, binding = 2) uniform _Lighting
{
								// size		// offset
	vec4 _Ambient;				// 16		// 0
	Light _Lights[LIGHTS_MAX];	// 80*5		// 16
	int _NumLights;				// 4		// 416
};

layout(std140, binding = 3) uniform _Fog
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
layout (binding = 0) uniform sampler2D _Diffuse;		// diffuse
layout (binding = 1) uniform sampler2D _NormalMap;		// normal map
layout (binding = 2) uniform sampler2D _SpecMap;		// specular map
layout (binding = 15) uniform samplerCube _Cubemap;		// cubemap
uniform vec4 _AlbedoColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform float _NormalMapScale = 1.0;

// specular
uniform float _SpecularStrength = 1;
uniform float _Smoothness = 0.9;
uniform float _Reflectivity = 0.1;

//inout
in VS_OUT
{
	in vec3 FragPos;
	in mat3 TBN;
	in vec2 TexCoord;
} fs_in;

layout (location = 0) out vec4 FragColour;

// functions
float CalculateSpecularIntensity(vec3 norm, vec3 lightDir, vec3 fragDir, float radius)
{
	// specular highlight
	float t = _Smoothness * texture(_SpecMap, fs_in.TexCoord).r;
	float exponent = mix(2, 1024, t);
	vec3 halfwayDir = normalize(lightDir + fragDir);
	float spec = pow(max(dot(-halfwayDir, norm), 0.0), exponent);

	return _SpecularStrength * spec;
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

vec3 CalculateLights(vec3 norm, vec3 fragDir)
{
	vec3 lightValue = vec3(0, 0, 0);
	for (int i = 0; i < _NumLights; i++)
	{
		switch(_Lights[i].type)
		{
			case 0:
				lightValue += CalculateDirectionalLight(i, norm, fragDir);
			break;
			case 1:
				lightValue += CalculatePointLight(i, norm, fragDir);
			break;
		}
	}

	return lightValue;
}

vec3 CalculateCubemapColor(vec3 norm, vec3 fragDir)
{
	vec3 eyeDir = fragDir;
	vec3 reflected = normalize(reflect(eyeDir, norm));

	vec3 transformed = mat3(_iVmat) * reflected;
	
	return texture(_Cubemap, -transformed).rgb * _Reflectivity;
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
	vec3 norm = texture(_NormalMap, fs_in.TexCoord).rgb;
	norm = norm * 2.0 - 1.0;
	norm = normalize(fs_in.TBN * norm);

	vec3 viewDir = vec3(0, 0, 1);
	vec3 fragDir = normalize(fs_in.FragPos);

	vec3 totalLight;
	totalLight += CalculateLights(norm, fragDir);
	totalLight += _Ambient.rgb;

	vec4 color = texture(_Diffuse, fs_in.TexCoord) * _AlbedoColor * vec4(totalLight, 1.0);
	color += vec4(CalculateCubemapColor(norm, fragDir), 0) * texture(_SpecMap, fs_in.TexCoord).r;
	
	// do fog
	FragColour = ProcessFog(color);
}