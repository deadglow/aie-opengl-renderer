#version 460

#define SAMPLE_DELTA 0.025

in vec3 localPos;
out vec4 FragColor;

layout (binding = 15) uniform samplerCube _Skybox;


void main()
{
	vec3 normal = normalize(localPos);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, normal));
	up = normalize(cross(normal, right));

	vec3 irradiance = vec3(0.0);
	int numSamples = 0;
	const float PI = 3.14159265359;

	for (float phi = 0.0; phi < 2.0 * PI; phi += SAMPLE_DELTA)
	{
		for (float theta = 0.0; theta < 0.5 * PI; theta += SAMPLE_DELTA)
		{
			// sphereical to cartesian
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos (theta));
			
			// tangent to world
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

			irradiance += texture(_Skybox, sampleVec).rgb * cos(theta) * sin(theta);
			numSamples++;
		}
	}
	
	irradiance = PI * irradiance * (1.0 / float(numSamples));
	FragColor = vec4(irradiance, 1.0);
}