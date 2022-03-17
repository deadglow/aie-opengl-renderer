#pragma once
#include <string>
#include "glm.hpp"
#include "glad.h"

class Shader
{
private:
	std::string name;
	unsigned int ID;

public:
	Shader(unsigned int ID_init, std::string name_init);
	~Shader();

	void Use();

	const unsigned int GetID();
	const std::string GetName();

	const int GetUniformLocation(const std::string variable) const;

	void SetUniform(const std::string variable, float value) const;
	void SetUniform(const std::string variable, int value) const;
	void SetUniform(const std::string variable, bool value) const;
	void SetUniform(const std::string variable, const glm::vec3 value) const;
	void SetUniform(const std::string variable, const glm::vec4 value) const;
	void SetUniform(const std::string variable, glm::mat4 value) const;
};

