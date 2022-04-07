#pragma once
#include "Model.h"
#include <string>

struct aiNode;
struct aiMesh;
struct aiScene;

class ModelLoadFunctions
{
public:
	static std::string dir;

	static bool LoadModelFromFile(Model* model);
	static void ProcessNode(Model* model, aiNode* node, const aiScene* scene);
	static Mesh* ProcessMesh(Model* model, aiMesh* mesh, glm::mat4 transform, const aiScene* scene);
};

