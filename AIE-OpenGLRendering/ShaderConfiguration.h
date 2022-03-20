#pragma once
#include "Shader.h"
#include "ShaderProperty.h"
#include <vector>
#include <string>


class ShaderConfiguration
{
private:
	Shader* shader = nullptr;
	std::string shaderName;
	std::vector<ShaderPropertyBase*> properties;

public:
	ShaderConfiguration(Shader* shader_init);
	~ShaderConfiguration();

	template<class T>
	void AddProperty(const std::string name, const T value)
	{
		ShaderProperty<T>* prop = new ShaderProperty<T>(name, value);
		properties.push_back(prop);
	}

	void UseShader();
	void ApplyConfiguration();
};
