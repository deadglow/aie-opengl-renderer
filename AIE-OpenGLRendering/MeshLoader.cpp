#include "MeshLoader.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::string MeshLoader::dir = fs::current_path().string() + "/meshes";
std::unordered_map<std::string, Mesh*> MeshLoader::meshList;

// DO THIS DO THIS DO THIS
Mesh* MeshLoader::CreateMesh(const std::string filepath)
{
	return nullptr;
}

void MeshLoader::Initialise()
{
	// find all mesh files in the meshes folder
	if (fs::exists(dir) && fs::is_directory(dir))
	{
		for (fs::directory_entry const& entry : fs::recursive_directory_iterator(dir))
		{
			if (!fs::is_regular_file(entry)) continue;

			fs::path path = entry.path();
			std::string extension = path.filename().extension().string();
			if (extension == ".obj")
			{
				Mesh* mesh = CreateMesh(path.string());
				meshList.emplace(path.filename().string(), mesh);
				continue;
			}
		}
	}
}

void MeshLoader::Shutdown()
{
	for (auto const [key, value] : meshList)
	{
		delete value;
	}

	meshList.clear();
	std::cout << "Meshes unloaded." << std::endl;
}

void MeshLoader::PrintAllMeshFiles()
{
	std::cout << "Mesh files: " << std::endl;
	for (auto const [key, value] : meshList)
	{
		std::cout << key << "\n" << std::endl;
	}
}

Mesh* MeshLoader::GetMesh(const std::string filename)
{
	return meshList.at(filename);
}

const Mesh* MeshLoader::LoadMesh(const std::string filename)
{
	Mesh* mesh = GetMesh(filename);
	mesh->LoadMesh();

	return mesh;
}

void MeshLoader::UnloadMesh(const std::string filename)
{
	Mesh* mesh = GetMesh(filename);
	mesh->UnloadMesh();
}
