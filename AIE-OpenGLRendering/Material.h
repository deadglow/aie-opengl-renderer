#pragma once
#include "Shader.h"
#include "ShaderProperty.h"
#include <vector>
#include <string>


class Material
{
private:
	Shader* shader = nullptr;
	std::string shaderName;
	std::vector<ShaderPropertyBase*> properties;
	std::vector<Texture*> usedTextures;

public:
	Material(Shader* shader_init);
	~Material();

	template<class T>
	void AddProperty(const std::string name, const T value)
	{
		ShaderProperty<T>* prop = new ShaderProperty<T>(name, value);
		properties.push_back(prop);
	}

	void AddTexture(Texture* texture);

	void UseShader();
	void ApplyConfiguration();
};

