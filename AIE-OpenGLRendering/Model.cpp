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

void Model::SetMaterial(const int index, Material* shaderConfig)
{
	defaultMaterials[index] = shaderConfig;
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
	if (!loaded) throw std::runtime_error("Model not loaded.");

	for (int i = 0; i < meshes.size(); ++i)
	{
		// make matrices
		glm::mat4 t = transform * meshes[i]->transform;
		glm::mat4 inverseTransform = glm::inverse(t);
		glm::mat4 normalMatrix = csd.vMatrix * t;
		normalMatrix[3] = { 0, 0, 0, 1 };

		defaultMaterials[i]->UseShader();
		meshes[i]->Draw();
	}
}

void Model::AddToDrawCall(glm::mat4 transform, std::vector<Material*>* materialLookup)
{
	if (!loaded) throw std::runtime_error("Model not loaded.");

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
