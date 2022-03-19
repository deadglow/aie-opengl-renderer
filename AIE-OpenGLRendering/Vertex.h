#pragma once
#include "glm.hpp"

struct Vertex
{
	float pos[3];
	float normal[3];
	float texCoords[2];
	unsigned int color[4];

	Vertex() = default;
	Vertex(float pos_init[3], float normal_init[3], float texCoords_init[2], unsigned int colors_init[4]);
};
