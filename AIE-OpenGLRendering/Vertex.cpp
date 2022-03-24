#include "Vertex.h"

Vertex::Vertex(float pos_init[3], float normal_init[3], float tangent_init[3], float texCoord_init[2])
{
	memcpy_s(&pos, sizeof(pos), pos_init, sizeof(pos));
	memcpy_s(&normal, sizeof(normal), normal_init, sizeof(normal));
	memcpy_s(&tangent, sizeof(tangent), tangent_init, sizeof(tangent));
	memcpy_s(&texCoord, sizeof(texCoord), texCoord_init, sizeof(texCoord));
}

Vertex::Vertex(glm::vec3 pos_init, glm::vec3 normal_init, glm::vec3 tangent_init, glm::vec2 texCoord_init)
{
	pos = pos_init;
	normal = normal_init;
	tangent = tangent_init;
	texCoord = texCoord_init;
}
