#pragma once
#include "glm/glm.hpp"

struct LightShaderData
{
	glm::vec3 position = glm::vec3(0);
	float padding0 = 1.0f;
	glm::vec3 direction = glm::vec3(0, 0, 1);
	float padding1 = 0.0f;
	glm::vec4 color = glm::vec4(1);
	glm::vec4 properties = glm::vec4(1);
	int type = 0;
	float padding2[3] = { 0, 0, 0 };
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

