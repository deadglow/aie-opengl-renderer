#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight(glm::vec3 direction_init, glm::vec3 color_init, float intensity_init);
	DirectionalLight(const DirectionalLight& dirLight) = default;
	DirectionalLight() = default;
	~DirectionalLight() = default;

	LightType GetType();
	LightShaderData ConstructShaderData();
};

