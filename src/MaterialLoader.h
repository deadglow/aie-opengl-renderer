#pragma once
#include "Material.h"
#include <string>
#include <unordered_map>

class MaterialLoader
{
private:
	static std::string dir;

	MaterialLoader() = delete;
	~MaterialLoader() = delete;
public:
	static std::unordered_map<std::string, Material*> materialLookup;

	static void Initialise();
	static void Shutdown();
	static void PrintAllMaterials();
	static Material* BuildMaterial(std::string filepath);
	
	static Material* GetMaterial(const std::string filename);
};

