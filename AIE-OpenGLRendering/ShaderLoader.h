#pragma once
#include <unordered_map>
#include <string>
#include "Shader.h"

class ShaderLoader
{
private:
	std::string dir;
	std::string shaderListFilename;
	std::unordered_map<std::string, unsigned int> vertexShaders;
	std::unordered_map<std::string, unsigned int> fragmentShaders;
	std::unordered_map<std::string, Shader*> shaderPrograms;

	bool shaderStateOkay = false;
	Shader* currentShader = nullptr;

	void UseShader(Shader* shader);
	bool LoadInShaders();
	void DeleteVertexAndFragmentShaders();
	void ClearShaders();

public:
	ShaderLoader();
	ShaderLoader(std::string dir_init, std::string shaderListFilename_init);

	~ShaderLoader();

	const bool InitialiseShaders();
	Shader* GetCurrentShader() const;
	void UseShader(const std::string shader);
	void PrintShaderCollections() const;

	const bool GetShaderStateOkay() const { return shaderStateOkay; }

};

