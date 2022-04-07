#pragma once
#include "glm/glm.hpp"
#include <string>
#include <unordered_set>

class Texture2D;

class Shader
{
private:
	std::string name;
	unsigned int ID;
	int textureCount = 0;

public:
	Shader(unsigned int ID_init, std::string name_init);
	~Shader();

	void Use();

	const unsigned int GetID() const;
	const int GetTextureCount() const;
	void SetTextureCount(const int value);
	const std::string GetName() const;

	const int GetUniformLocation(const std::string variable) const;

	void SetUniform(const std::string variable, float value) const;
	void SetUniform(const std::string variable, int value) const;
	void SetUniform(const std::string variable, unsigned int value) const;
	void SetUniform(const std::string variable, bool value) const;
	void SetUniform(const std::string variable, const glm::vec2 value) const;
	void SetUniform(const std::string variable, const glm::vec3 value) const;
	void SetUniform(const std::string variable, const glm::vec4 value) const;
	void SetUniform(const std::string variable, glm::mat2 value) const;
	void SetUniform(const std::string variable, glm::mat3 value) const;
	void SetUniform(const std::string variable, glm::mat4 value) const;
};

