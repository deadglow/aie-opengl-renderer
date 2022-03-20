#pragma once
#include "Shader.h"
#include <string>
#include <iostream>

struct ShaderPropertyBase
{
	std::string name;

	ShaderPropertyBase() = default;
	virtual ~ShaderPropertyBase() = default;
	ShaderPropertyBase(const std::string name_init)
	{ 
		name = name_init; 
	}
	virtual void SetProperty(Shader* shader) { std::cout << "Property not of any type!" << std::endl; };
};

template<class T>
struct ShaderProperty :
	ShaderPropertyBase
{
	T value;

	ShaderProperty<T>() = default;
	ShaderProperty<T>(const std::string name_init, T value_init) : ShaderPropertyBase(name_init)
	{
		value = value_init;
	}
	~ShaderProperty<T>() = default;

	void SetProperty(Shader* shader)
	{
		shader->SetUniform(name, (T)value);
	}
};