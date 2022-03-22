#pragma once
#include "glm.hpp"

class DirectionalLight
{
public:
	glm::vec3 direction;
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
	float intensity = 1.0f;

	DirectionalLight(glm::vec3 direction_init, glm::vec3 color_init, float intensity_init);
	DirectionalLight();
	~DirectionalLight();

	glm::vec4 GetFinalColor();
};

