#pragma once
#include <unordered_map>
#include <string>
#include "Model.h"


class ModelLoader
{
private:
	static std::string dir;

	static Model* CreateModel(const std::string filepath, const std::string filename);

	ModelLoader() = delete;
public:
	static std::unordered_map<std::string, Model*> modelList;

	static void Initialise();
	static void Shutdown();
	static void PrintAllMeshFiles();

	static Model* GetModel(const std::string filename);
	static Model* LoadModel(const std::string filename);
	static void UnloadModel(const std::string filename);
};

