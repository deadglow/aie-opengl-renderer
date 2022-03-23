#pragma once
#include "glm/glm.hpp"

struct CameraShaderData
{
	glm::mat4 pMatrix;
	glm::mat4 ipMatrix;
	glm::mat4 vMatrix;
	glm::mat4 ivMatrix;
};