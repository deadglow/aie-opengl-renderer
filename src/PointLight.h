#pragma once
#include "Light.h"

class PointLight
	: public Light
{
public:
	float constant = 1.0f;
	float linear = 1.0f;
	float quadratic = 1.0f;
	float radius = 0.1f;

	PointLight(glm::vec3 position_init, glm::vec3 color_init, float intensity_init);
	PointLight(const PointLight& pLight) = default;
	PointLight() = default;
	~PointLight() = default;

	LightType GetType();

	LightShaderData ConstructShaderData();

	glm::vec4 GetProperties();
};

