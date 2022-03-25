#pragma once
#include "Light.h"

class PointLight
	: public Light
{
public:
	glm::vec3 position{ 0, 0, 0 };
	float constant = 1.0f;
	float linear = 1.0f;
	float quadratic = 1.0f;
	float radius = 0.1f;

	PointLight(glm::vec3 direction_init, glm::vec3 color_init, float intensity_init);
	PointLight(const PointLight& pLight);
	PointLight();
	~PointLight();

	int GetType();

	LightShaderData ConstructShaderData();

	glm::vec4 GetProperties();
};

