#pragma once
#include <unordered_map>
#include <string>
#include "ShaderConfiguration.h"

class ShaderLoader
{
private:
	static std::string dir;
	static std::string shaderListFilename;
	static std::unordered_map<std::string, unsigned int> vertexShaders;
	static std::unordered_map<std::string, unsigned int> fragmentShaders;
	static std::unordered_map<std::string, Shader*> shaderPrograms;

	static bool shaderStateOkay;
	static Shader* currentShader;

	static bool LoadInShaders();
	static void DeleteVertexAndFragmentShaders();
	static void ClearShaders();

	ShaderLoader() = delete;
public:


	static const bool InitialiseShaders();
	static void Shutdown();
	static Shader* GetCurrentShader();
	static Shader* GetShader(const std::string filename);
	static void UseShader(Shader* shader);
	static void UseShader(const std::string shader);
	static void PrintShaderCollections();

	static const bool GetShaderStateOkay() { return shaderStateOkay; }

};

