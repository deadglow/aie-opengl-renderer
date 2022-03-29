#include "Material.h"
#include "ShaderLoader.h"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Renderer.h"

Material::Material(Shader* shader_init, const std::string name_init)
{
	shader = shader_init;
	shaderName = shader->GetName();
	materialName = name_init;
}

Material::~Material()
{
	for (int i = 0; i < properties.size(); ++i)
	{
		delete properties[i];
	}
	properties.clear();
}

void Material::AddTexture(Texture* texture)
{
	usedTextures.push_back(texture);
}

void Material::UseShader()
{
	// we get it by name so hot reloading works (shader object would go poof on a reload)
	ShaderLoader::GetShader(shaderName)->Use();
	ApplyConfiguration();
}

void Material::ApplyConfiguration()
{
	for (int i = 0; i < properties.size(); ++i)
	{
		properties[i]->SetProperty(shader);
	}

	// set texture bindings
	for (int i = 0; i < usedTextures.size(); ++i)
	{
		TEX_Type type = usedTextures[i]->GetTexType();

		glActiveTexture(GL_TEXTURE0 + i);

		switch (type)
		{
		case TEX_Type::Texture2D:
		{
			glBindTexture(GL_TEXTURE_2D, usedTextures[i]->GetID());

		}
		break;

		case TEX_Type::Cubemap:
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, usedTextures[i]->GetID());
		}
		break;
		}
	}
}

Shader* Material::GetShader() const
{
	return ShaderLoader::GetShader(shaderName);
}

const std::string Material::GetName() const
{
	return materialName;
}
