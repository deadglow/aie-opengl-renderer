#pragma once
#include "glm/glm.hpp"

struct alignas(16) LightShaderData
{
	alignas(16) glm::vec3 position = glm::vec3(0);
	alignas(16) glm::vec3 direction = glm::vec3(0, 0, 1);
	alignas(16) glm::vec4 color = glm::vec4(1);
	alignas(16) glm::vec4 properties = glm::vec4(1);
	alignas(4) int type = 0;
};

class Light
{
public:
	glm::vec3 color{ 1.0f, 1.0f, 1.0f };
	float intensity =  1.0f;

	Light(glm::vec3 color_init, float intensity_init);
	Light();
	virtual ~Light();

	virtual int GetType();

	glm::vec4 GetFinalColor();

	virtual LightShaderData ConstructShaderData();
};

