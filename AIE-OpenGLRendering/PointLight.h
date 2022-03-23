#pragma once
#include "glm/glm.hpp"

class PointLight
{
public:
	glm::vec3 position;
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
	float intensity = 1.0f;
	float constant = 1.0f;
	float linear = 1.0f;
	float quadratic = 1.0f;
	float radius = 0.1f;

	PointLight();
	~PointLight();

	glm::vec4 GetFinalColor();
	glm::vec4 GetProperties();
};

