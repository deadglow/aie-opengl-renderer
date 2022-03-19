#pragma once
#include "Shader.h"
#include "ShaderProperty.h"
#include <vector>
#include <string>


class ShaderConfiguration
{
private:
	Shader* shader = nullptr;
	std::vector<ShaderPropertyBase> properties;

public:
	ShaderConfiguration(Shader* shader_init);

	template<class T>
	void AddProperty(const std::string name, T value)
	{
		ShaderProperty<T> prop;
		prop.name = name;
		prop.value = value;
		properties.push_back(prop);
	}

	void UseShader();
};

