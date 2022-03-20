#pragma once
#include "glm.hpp"

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;
	unsigned char color[4];

	Vertex() = default;
	Vertex(glm::vec3 pos_init, glm::vec3 normal_init, glm::vec2 texCoord_init, unsigned char color_init[4]);
	Vertex(float pos_init[3], float normal_init[3], float texCoord_init[2], unsigned char color_init[4]);
};
