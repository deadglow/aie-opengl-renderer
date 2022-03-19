#pragma once
#include "Shader.h"
#include <string>

struct ShaderPropertyBase
{
	std::string name;

	virtual void SetProperty(Shader* shader) = 0;
};

template<class T>
struct ShaderProperty :
	ShaderPropertyBase
{
	std::string name;
	T value;

	void SetProperty(Shader* shader)
	{
		shader->SetUniform(name, (T)value);
	}
};