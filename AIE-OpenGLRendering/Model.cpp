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
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(transform));
	for (int i = 0; i < meshes.size(); ++i)
	{
		shaderConfigs[meshShaderLookup[i]]->UseShader();
		ShaderLoader::GetCurrentShader()->SetUniform("_VP", csd.vpMatrix);
		ShaderLoader::GetCurrentShader()->SetUniform("_iVP", csd.ivpMatrix);
		ShaderLoader::GetCurrentShader()->SetUniform("_M2W", transform);
		ShaderLoader::GetCurrentShader()->SetUniform("_NormalMatrix", normalMatrix);
		ShaderLoader::GetCurrentShader()->SetUniform("_CamPos", csd.position);
		ShaderLoader::GetCurrentShader()->SetUniform("_CamDir", csd.direction);
		ShaderLoader::GetCurrentShader()->SetUniform("_NearZ", csd.nearz);
		ShaderLoader::GetCurrentShader()->SetUniform("_FarZ", csd.farz);
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
