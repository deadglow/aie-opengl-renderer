#pragma once
#include "Mesh.h"
#include "CameraShaderData.h"

struct MeshDrawData
{
	Mesh* mesh;

	// matrices
	glm::mat4 m2w;
	glm::mat4 w2m;

	MeshDrawData(Mesh* mesh_init, glm::mat4 m2w_init, glm::mat4 w2m_init);
	~MeshDrawData();

	void Draw(CameraShaderData csd);
};

