#pragma once
#include "Shader.h"
#include "ShaderProperty.h"
#include "Texture2D.h"
#include "Cubemap.h"
#include <vector>
#include <string>


class Material
{
private:
	Shader* shader = nullptr;
	std::string shaderName;
	std::string materialName;

public:
	std::vector<Texture*> usedTextures;
	std::vector<ShaderPropertyBase*> properties;

	Material(Shader* shader_init, const std::string name_init);
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

	Shader* GetShader() const;
	const std::string GetName() const;
};

