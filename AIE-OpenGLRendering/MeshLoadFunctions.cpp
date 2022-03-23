#include "MeshLoadFunctions.h"
#include "FileReader.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

// do this do this
Mesh* MeshLoadFunctions::CreateMeshFromFile(const std::string filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
	return nullptr;
}
