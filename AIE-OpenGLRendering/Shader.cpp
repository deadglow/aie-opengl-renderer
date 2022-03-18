#include "Shader.h"
#include "glad.h"
#include "glfw3.h"

Shader::Shader(unsigned int ID_init, std::string name_init)
{
	ID = ID_init;
	name = name_init;
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::Use()
{
	glUseProgram(ID);
}

const unsigned int Shader::GetID()
{
	return ID;
}

const std::string Shader::GetName()
{
	return name;
}

const int Shader::GetUniformLocation(const std::string variable) const
{
	return glGetUniformLocation(ID, variable.c_str());
}

void Shader::SetUniform(const std::string variable, float value) const
{
	glUniform1f(GetUniformLocation(variable), value);
}

void Shader::SetUniform(const std::string variable, int value) const
{
	glUniform1i(GetUniformLocation(variable), value);
}

void Shader::SetUniform(const std::string variable, bool value) const
{
	glUniform1i(GetUniformLocation(variable), (int)value);
}

void Shader::SetUniform(const std::string variable, const glm::vec3 value) const
{
	glUniform3f(GetUniformLocation(variable), value.x, value.y, value.z);
}

void Shader::SetUniform(const std::string variable, const glm::vec4 value) const
{
	glUniform4f(GetUniformLocation(variable), value.x, value.y, value.z, value.w);
}

void Shader::SetUniform(const std::string variable, glm::mat4 value) const
{
	glUniformMatrix4fv(GetUniformLocation(variable), 1, false, &value[0][0]);
}