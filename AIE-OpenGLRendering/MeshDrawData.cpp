#include "MeshDrawData.h"
#include "ShaderLoader.h"

MeshDrawData::MeshDrawData(Mesh* mesh_init, glm::mat4 m2w_init, glm::mat4 w2m_init)
{
	mesh = mesh_init;
	m2w = m2w_init;
	w2m = w2m_init;
}

MeshDrawData::~MeshDrawData()
{
}

void MeshDrawData::Draw(CameraShaderData csd)
{
	ShaderLoader::GetCurrentShader()->SetUniform("_M2W", m2w);
	ShaderLoader::GetCurrentShader()->SetUniform("_W2M", w2m);
	// calculate view space normal matrix
	glm::mat4 normalMatrix = csd.vMatrix * m2w;
	normalMatrix[3] = { 0, 0, 0, 1 };
	ShaderLoader::GetCurrentShader()->SetUniform("_NormalMat", normalMatrix);

	mesh->Draw();
}
