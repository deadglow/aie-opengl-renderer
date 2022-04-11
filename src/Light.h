#pragma once
#include "Transform.h"
#include "glm/glm.hpp"
#include <string>

struct alignas(16) LightShaderData
{
	alignas(16) glm::vec3 position = glm::vec3(0);
	alignas(16) glm::vec3 direction = glm::vec3(0, 0, 1);
	alignas(16) glm::vec4 color = glm::vec4(1);
	alignas(16) glm::vec4 properties = glm::vec4(1);
	alignas(16) int type = 0;
};

enum class LightType : int
{
	Directional,
	Point,
	Spot
};

class Light
{
public:
	Transform transform;
	glm::vec3 color{ 1.0f, 1.0f, 1.0f };
	float intensity =  1.0f;

	Light(glm::vec3 color_init, float intensity_init);
	Light() = default;
	virtual ~Light() = default;

	virtual LightType GetType();

	glm::vec4 GetFinalColor();

	virtual LightShaderData ConstructShaderData();

	static std::string GetTypeName(LightType type);
};

