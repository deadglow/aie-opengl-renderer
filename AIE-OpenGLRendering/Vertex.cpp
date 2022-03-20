#include "Vertex.h"

Vertex::Vertex(float pos_init[3], float normal_init[3], float texCoord_init[2], unsigned char color_init[4])
{
	memcpy_s(&pos, sizeof(pos), pos_init, sizeof(pos));
	memcpy_s(&normal, sizeof(normal), normal_init, sizeof(normal));
	memcpy_s(&texCoord, sizeof(texCoord), texCoord_init, sizeof(texCoord));
	memcpy_s(&color, sizeof(color), color_init, sizeof(color));
}

Vertex::Vertex(glm::vec3 pos_init, glm::vec3 normal_init, glm::vec2 texCoord_init, unsigned char color_init[4])
{
	pos = pos_init;
	normal = normal_init;
	texCoord = texCoord_init;
	memcpy_s(&color, sizeof(color), color_init, sizeof(color));
}
