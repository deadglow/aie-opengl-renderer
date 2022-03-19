#include "ShaderConfiguration.h"
#include "glad.h"
#include "glfw3.h"

ShaderConfiguration::ShaderConfiguration(Shader* shader_init)
{
	shader = shader_init;
}

void ShaderConfiguration::UseShader()
{
	shader->Use();
	for (int i = 0; i < properties.size(); ++i)
	{
		properties[i].SetProperty(shader);
	}
}
