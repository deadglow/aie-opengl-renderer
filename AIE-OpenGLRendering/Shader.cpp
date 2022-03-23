#include "Shader.h"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Texture.h"
#include "ShaderLoader.h"
#include "glm/gtc/type_ptr.hpp"

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
	ShaderLoader::UseShader(this);
}

const unsigned int Shader::GetID() const
{
	return ID;
}

const std::string Shader::GetName() const
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

void Shader::SetUniform(const std::string variable, unsigned int value) const
{
	glUniform1ui(GetUniformLocation(variable), value);
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
	glUniformMatrix4fv(GetUniformLocation(variable), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string variable, Texture* tex) const
{
	SetUniform(variable, tex->GetID());
}
