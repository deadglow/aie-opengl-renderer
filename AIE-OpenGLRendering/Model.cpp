#include "Model.h"
#include "ShaderLoader.h"

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
		delete meshes[i];
	}
	meshes.clear();
}

void Model::AddMesh(Mesh* mesh)
{
	meshes.push_back(mesh);
	meshShaderLookup.push_back(0);
}

void Model::AddShaderConfig(ShaderConfiguration* shaderConfig)
{
	shaderConfigs.push_back(shaderConfig);
}

void Model::Load()
{
	if (!loaded)
	{
		for (int i = 0; i < meshes.size(); ++i)
		{
			meshes[i]->LoadMesh();
		}
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
	}
}

void Model::Draw(CameraShaderData csd, glm::mat4 transform)
{
	for (int i = 0; i < meshes.size(); ++i)
	{
		// make matrices
		glm::mat4 t = transform * meshes[i]->transform;
		glm::mat4 inverseTransform = glm::inverse(t);
		glm::mat4 normalMatrix = csd.vMatrix * t;
		normalMatrix[3] = { 0, 0, 0, 1 };

		shaderConfigs[meshShaderLookup[i]]->UseShader();
		ShaderLoader::GetCurrentShader()->SetUniform("_M2W", t);
		ShaderLoader::GetCurrentShader()->SetUniform("_W2M", inverseTransform);
		ShaderLoader::GetCurrentShader()->SetUniform("_NormalMat", normalMatrix);
		meshes[i]->Draw();
	}
}

void Model::SetShaderOfMesh(const int meshIndex, const int shaderIndex)
{
	meshShaderLookup[meshIndex] = shaderIndex;
}

std::string Model::GetFilename() const
{
	return filename;
}

const bool Model::IsLoaded() const
{
	return loaded;
}
