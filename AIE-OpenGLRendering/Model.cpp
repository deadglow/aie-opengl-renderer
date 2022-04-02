#include "Model.h"
#include "ShaderLoader.h"
#include "MeshDrawData.h"
#include "Renderer.h"

Model::Model(const std::string filename_init)
{
	filename = filename_init;
}

Model::~Model()
{
	if (loaded)
	{
		Unload();
	}

	for (int i = 0; i < meshes.size(); ++i)
	{
		Mesh* mesh = meshes[i];
		if (mesh)
			delete mesh;
	}
	meshes.clear();
}

void Model::AddMesh(Mesh* mesh)
{
	meshes.push_back(mesh);
	defaultMaterials.resize(meshes.size());
}

void Model::SetMaterial(const int index, Material* material)
{
	defaultMaterials[index] = material;
}

void Model::SetAllMaterials(Material* material)
{
	for (int i = 0; i < meshes.size(); ++i)
	{
		SetMaterial(i, material);
	}
}

void Model::Load()
{
	if (!loaded)
	{
		for (int i = 0; i < meshes.size(); ++i)
		{
			meshes[i]->LoadMesh();
		}
		loaded = true;
	}
}

void Model::Unload()
{
	if (loaded)
	{
		for (int i = 0; i < meshes.size(); ++i)
		{
			meshes[i]->UnloadMesh();
		}
		loaded = false;
	}
}

void Model::Draw(CameraShaderData csd, glm::mat4 transform)
{
	if (!loaded)
	{
		ModelLoader::GetModel(MODEL_DEFAULT)->Draw(csd, transform);
		return;
	}

	for (int i = 0; i < meshes.size(); ++i)
	{

		// make matrices
		glm::mat4 m2w = transform * meshes[i]->transform;
		glm::mat4 w2m = glm::inverse(m2w);
		glm::mat4 normalMatrix = csd.vMatrix * m2w;
		normalMatrix[3] = { 0, 0, 0, 1 };

		defaultMaterials[i]->UseMaterial();
		ShaderLoader::GetCurrentShader()->SetUniform("_M2W", m2w);
		ShaderLoader::GetCurrentShader()->SetUniform("_W2M", w2m);
		ShaderLoader::GetCurrentShader()->SetUniform("_NormalMat", normalMatrix);
		meshes[i]->Draw();
	}
}

void Model::AddToDrawCall(glm::mat4 transform, std::vector<Material*>* materialLookup)
{
	if (!loaded)
	{
		ModelLoader::GetModel(MODEL_DEFAULT)->AddToDrawCall(transform);
		return;
	}

	for (int i = 0; i < meshes.size(); ++i)
	{
		glm::mat4 t = transform * meshes[i]->transform;
		glm::mat4 inverseTransform = glm::inverse(t);
		MeshDrawData drawData(meshes[i], t, inverseTransform);
		Renderer::AddToDrawCall(materialLookup->at(i), drawData);
	}
}

void Model::AddToDrawCall(glm::mat4 transform)
{
	AddToDrawCall(transform, &defaultMaterials);
}

std::string Model::GetFilename() const
{
	return filename;
}

const bool Model::IsLoaded() const
{
	return loaded;
}
