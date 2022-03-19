#pragma once
#include <unordered_map>
#include <string>
#include "Mesh.h"


class MeshLoader
{
private:
	static std::string dir;
	static std::unordered_map<std::string, Mesh*> meshList;

	static Mesh* CreateMesh(const std::string filepath);

	MeshLoader() = delete;
public:

	static void Initialise();
	static void Shutdown();
	static void PrintAllMeshFiles();

	static Mesh* GetMesh(const std::string filename);
	static const Mesh* LoadMesh(const std::string filename);
	static void UnloadMesh(const std::string filename);
};

