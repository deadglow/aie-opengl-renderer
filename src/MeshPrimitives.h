#pragma once
#include "Mesh.h"
#include "glm/glm.hpp"

class MeshPrimitives
{
public:
	
	MeshPrimitives() = delete;

	static Mesh* CreatePlane(float width, float height, glm::vec3 normal, glm::vec3 tangent);
	static Mesh* CreateCube(float width, float height, float depth);
	static Mesh* CreateSphere(float radius);
	static Mesh* CreateCylinder(float radius, float height);
	static Mesh* CreateBean(float radius, float height);
};

