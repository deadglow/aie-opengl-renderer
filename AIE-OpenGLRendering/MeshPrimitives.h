#pragma once
#include "Mesh.h"
#include "glm/glm.hpp"

class MeshPrimitives
{
public:
	MeshPrimitives() = delete;

	static Mesh* CreateCube(float width, float height, float depth);
	static Mesh* CreateSphere(float radius);
	static Mesh* CreateCylinder(float radius, float height);
	static Mesh* CreateSausage(float radius, float height);
};

