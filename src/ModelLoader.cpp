#include "ModelLoader.h"
#include "ModelLoadFunctions.h"
#include "FileReader.h"
#include "Renderer.h"
#include "MaterialLoader.h"
#include <filesystem>
#include <iostream>
#include <vector>


std::unordered_map<std::string, Model*> ModelLoader::modelList;

void ModelLoader::Initialise()
{
	std::vector<fs::path> paths;
	std::vector<std::string> extensions{ ".obj", ".fbx", ".blend" };

	// find all mesh files in the meshes folder
	FileReader::GetAllFilesWithExtensions(ModelLoadFunctions::dir, &extensions, &paths);

	for (int i = 0; i < paths.size(); ++i)
	{
		Model* model = new Model(paths[i].filename().string());
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

Model* ModelLoader::LoadModel(Model* model)
{
	bool success = ModelLoadFunctions::LoadModelFromFile(model);
	if (!success)
	{
		std::cout << "Failed to load model " << model->GetFilename() << std::endl;
		return nullptr;
	}

	std::cout << "Loaded model " << model->GetFilename() << std::endl;
	model->SetAllMaterials(MaterialLoader::GetMaterial(MATERIAL_DEFAULT));
	model->ModelLoaded();
	return model;
}

Model* ModelLoader::LoadModel(const std::string filename)
{
	Model* model = GetModel(filename);
	if (!model)
		throw std::out_of_range("Model not found.");

	return LoadModel(model);
}

void ModelLoader::UnloadModel(const std::string filename)
{
	Model* model = GetModel(filename);
	model->Unload();
}
