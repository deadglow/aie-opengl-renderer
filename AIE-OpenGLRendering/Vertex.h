#pragma once
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 texCoord;

	Vertex() = default;
	Vertex(glm::vec3 pos_init, glm::vec3 normal_init, glm::vec3 tangent_init, glm::vec2 texCoord_init);
	Vertex(float pos_init[3], float normal_init[3], float tangent_init[3], float texCoord_init[2]);
};
