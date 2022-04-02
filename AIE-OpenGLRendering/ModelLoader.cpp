#include "ModelLoader.h"
#include "ModelLoadFunctions.h"
#include "FileReader.h"
#include "Renderer.h"
#include "MaterialLoader.h"
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

std::string ModelLoader::dir = fs::current_path().string() + "\\models";
std::unordered_map<std::string, Model*> ModelLoader::modelList;

Model* ModelLoader::CreateModel(const std::string filepath, const std::string filename)
{
	return MeshLoadFunctions::CreateModelFromFile(filepath, filename);
}

void ModelLoader::Initialise()
{
	std::vector<fs::path> paths;
	std::vector<std::string> extensions{ ".obj", ".fbx", ".blend"};

	// find all mesh files in the meshes folder
	FileReader::GetAllFilesWithExtensions(dir, &extensions, &paths);

	for (int i = 0; i < paths.size(); ++i)
	{
		Model* model = CreateModel(paths[i].string(), paths[i].filename().string());
		model->SetAllMaterials(MaterialLoader::GetMaterial(MATERIAL_DEFAULT));
		modelList.emplace(paths[i].filename().string(), model);
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
	if (modelList.count(filename))
		return modelList.at(filename);
	else
		return nullptr;
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
