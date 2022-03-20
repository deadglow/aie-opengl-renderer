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

void Model::Draw(glm::mat4 mvp)
{
	for (int i = 0; i < meshes.size(); ++i)
	{
		shaderConfigs[meshShaderLookup[i]]->UseShader();
		ShaderLoader::GetCurrentShader()->SetUniform("_MVP", mvp);
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
