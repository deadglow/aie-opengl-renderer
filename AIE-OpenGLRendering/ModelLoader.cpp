#include "ModelLoader.h"
#include "FileReader.h"
#include "ModelLoadFunctions.h"
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

std::string ModelLoader::dir = fs::current_path().string() + "/models";
std::unordered_map<std::string, Model*> ModelLoader::modelList;

Model* ModelLoader::CreateModel(const std::string filepath)
{
	return MeshLoadFunctions::CreateModelFromFile(filepath);
}

void ModelLoader::Initialise()
{
	// find all mesh files in the meshes folder
	if (fs::exists(dir) && fs::is_directory(dir))
	{
		for (fs::directory_entry const& entry : fs::recursive_directory_iterator(dir))
		{
			if (!fs::is_regular_file(entry)) continue;

			fs::path path = entry.path();
			std::string extension = path.filename().extension().string();
			if (extension == ".obj" || extension == ".fbx")
			{
				Model* model = CreateModel(path.string());
				modelList.emplace(path.filename().string(), model);
				continue;
			}
		}
	}
}

void ModelLoader::Shutdown()
{
	for (auto const [key, value] : modelList)
	{
		delete value;
	}

	modelList.clear();
	std::cout << "Models unloaded." << std::endl;
}

void ModelLoader::PrintAllMeshFiles()
{
	std::cout << "Mesh files: " << std::endl;
	for (auto const [key, value] : modelList)
	{
		std::cout << key << "\n" << std::endl;
	}
}

Model* ModelLoader::GetModel(const std::string filename)
{
	return modelList.at(filename);
}

Model* ModelLoader::LoadModel(const std::string filename)
{
	Model* model = GetModel(filename);
	model ->Load();

	return model;
}

void ModelLoader::UnloadModel(const std::string filename)
{
	Model* model = GetModel(filename);
	model->Unload();
}