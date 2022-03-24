#include "ShaderConfiguration.h"
#include "ShaderLoader.h"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Renderer.h"

ShaderConfiguration::ShaderConfiguration(Shader* shader_init)
{
	shader = shader_init;
	shaderName = shader->GetName();
}

ShaderConfiguration::~ShaderConfiguration()
{
	for (int i = 0; i < properties.size(); ++i)
	{
		delete properties[i];
	}
	properties.clear();
}

void ShaderConfiguration::AddTexture(Texture* texture)
{
	usedTextures.push_back(texture);
}

void ShaderConfiguration::UseShader()
{
	// we get it by name so hot reloading works (shader object would go poof on a reload)
	ShaderLoader::GetShader(shaderName)->Use();
	ApplyConfiguration();
}

void ShaderConfiguration::ApplyConfiguration()
{
	for (int i = 0; i < properties.size(); ++i)
	{
		properties[i]->SetProperty(shader);
	}

	// set texture bindings
	for (int i = 0; i < usedTextures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, usedTextures[i]->GetID());
	}
}
