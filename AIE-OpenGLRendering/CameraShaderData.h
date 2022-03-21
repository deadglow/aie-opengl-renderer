#pragma once
#include "glm.hpp"

struct CameraShaderData
{
	glm::mat4 vpMatrix;
	glm::mat4 ivpMatrix;
	glm::vec3 position;
	glm::vec3 direction;
	float nearz;
	float farz;
};