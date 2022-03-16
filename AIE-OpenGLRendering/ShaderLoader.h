#pragma once
#include "glad.h"
#include "glm.hpp"
#include <unordered_map>
#include <string>

class ShaderLoader
{
private:
	std::string dir;
	std::string shaderListFilename;
	std::unordered_map<std::string, GLuint> vertexShaders;
	std::unordered_map<std::string, GLuint> fragmentShaders;
	std::unordered_map<std::string, GLuint> shaderPrograms;

	bool shaderStateOkay = false;
	std::string currentShaderName = "";
	GLuint currentShader = -1;

	void UseShader(GLuint shader);
	void ClearShaders();

public:
	ShaderLoader();
	ShaderLoader(std::string dir_init, std::string shaderListFilename_init);

	~ShaderLoader();

	bool LoadShaders();
	void UseShader(std::string shader);

	bool GetShaderStateOkay() { return shaderStateOkay; }

	GLuint GetUniformLocation(std::string variable);

	void SetUniform(std::string variable, float value);
	void SetUniform(std::string variable, glm::mat4 value);
};

