#pragma once
#include <unordered_map>
#include <string>
#include "Model.h"


class ModelLoader
{
private:
	ModelLoader() = delete;
public:
	static std::unordered_map<std::string, Model*> modelList;

	static void Initialise();
	static void Shutdown();
	static void PrintAllMeshFiles();

	static Model* GetModel(const std::string filename);
	static Model* LoadModel(Model* model);
	static Model* LoadModel(const std::string model);
	static void UnloadModel(const std::string filename);
};