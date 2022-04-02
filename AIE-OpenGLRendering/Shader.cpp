#include "Shader.h"
#include "glad.h"
#include "GLFW/glfw3.h"
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

const int Shader::GetTextureCount() const
{
	return textureCount;
}

void Shader::SetTextureCount(const int value)
{
	textureCount = value;
}

const std::string Shader::GetName() const
{
	return name;
}

const int Shader::GetUniformLocation(const std::string variable) const
{
	const char* cstring = variable.c_str();
	int val = glGetUniformLocation(ID, cstring);
	return val;
}

void Shader::SetUniform(const std::string variable, float value) const
{
	glUniform1f(GetUniformLocation(variable), value);
}

void Shader::SetUniform(const std::string variable, int value) const
{
	glUniform1i(GetUniformLocation(variable), value);
}

void Shader::SetUniform(const std::string variable, unsigned int value) const
{
	glUniform1ui(GetUniformLocation(variable), value);
}

void Shader::SetUniform(const std::string variable, bool value) const
{
	glUniform1i(GetUniformLocation(variable), (int)value);
}

void Shader::SetUniform(const std::string variable, const glm::vec2 value) const
{
	glUniform2f(GetUniformLocation(variable), value.x, value.y);
}

void Shader::SetUniform(const std::string variable, const glm::vec3 value) const
{
	glUniform3f(GetUniformLocation(variable), value.x, value.y, value.z);
}

void Shader::SetUniform(const std::string variable, const glm::vec4 value) const
{
	glUniform4f(GetUniformLocation(variable), value.x, value.y, value.z, value.w);
}

void Shader::SetUniform(const std::string variable, glm::mat2 value) const
{
	glUniformMatrix2fv(GetUniformLocation(variable), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string variable, glm::mat3 value) const
{
	glUniformMatrix3fv(GetUniformLocation(variable), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string variable, glm::mat4 value) const
{
	glUniformMatrix4fv(GetUniformLocation(variable), 1, GL_FALSE, glm::value_ptr(value));
}