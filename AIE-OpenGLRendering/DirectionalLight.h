#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	glm::vec3 direction{ 0, -1, 0 };

	DirectionalLight(glm::vec3 direction_init, glm::vec3 color_init, float intensity_init);
	DirectionalLight(const DirectionalLight& dirLight);
	DirectionalLight();
	~DirectionalLight();

	int GetType();
	LightShaderData ConstructShaderData();
};

