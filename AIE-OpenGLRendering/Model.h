#pragma once
#include "Mesh.h"
#include <string>
#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"

class Model
{
private:
	std::vector<Mesh*> meshes;
	std::string dir;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

};

