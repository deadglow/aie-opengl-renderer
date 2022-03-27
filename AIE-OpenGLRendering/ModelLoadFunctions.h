#pragma once
#include "Model.h"
#include <string>

struct aiNode;
struct aiMesh;
struct aiScene;

class MeshLoadFunctions
{
public:
	static Model* CreateModelFromFile(const std::string filepath, const std::string filename);
	static void ProcessNode(Model* model, aiNode* node, const aiScene* scene);
	static Mesh* ProcessMesh(Model* model, aiMesh* mesh, glm::mat4 transform, const aiScene* scene);
};

