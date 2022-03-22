#pragma once
#include "glm.hpp"

class PointLight
{
public:
	glm::vec3 position;
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
	float intensity = 1.0f;
	float constant = 1.0f;
	float linear = 1.0f;
	float quadratic = 1.0f;

	PointLight();
	~PointLight();

	glm::vec4 GetFinalColor();
};

