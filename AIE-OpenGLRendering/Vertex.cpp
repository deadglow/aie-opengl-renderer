#include "Vertex.h"

Vertex::Vertex(float pos_init[3], float normal_init[3], float texCoords_init[2], unsigned int colors_init[4])
{
	memcpy_s(pos, sizeof(pos), pos_init, sizeof(pos));
	memcpy_s(normal, sizeof(normal), pos_init, sizeof(normal));
	memcpy_s(texCoords, sizeof(texCoords), pos_init, sizeof(texCoords));
	memcpy_s(color, sizeof(color), pos_init, sizeof(color));
}
