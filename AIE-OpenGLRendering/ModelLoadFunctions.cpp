#include "ModelLoadFunctions.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>

// do this do this
Model* MeshLoadFunctions::CreateModelFromFile(const std::string filepath, const std::string filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "assimp error: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	Model* model = new Model(filename);

	ProcessNode(model, scene->mRootNode, scene);

	return model;
}

void MeshLoadFunctions::ProcessNode(Model* model, aiNode* node, const aiScene* scene)
{
	// process all node meshes
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		glm::mat4 transform;
		memcpy_s(&transform, sizeof(glm::mat4), &(node->mTransformation), sizeof(glm::mat4));
		model->AddMesh(ProcessMesh(model, mesh, transform, scene));
	}

	// recursively continue
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(model, node->mChildren[i], scene);
	}
}

Mesh* MeshLoadFunctions::ProcessMesh(Model* model, aiMesh* mesh, glm::mat4 transform, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;

	// process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		aiVector3D vec = mesh->mVertices[i];
		vertex.pos = { vec.x, vec.y, vec.z };
		vec = mesh->mNormals[i];
		vertex.normal = { vec.x, vec.y, vec.z };
		vec = mesh->mTangents[i];
		vertex.tangent = { vec.x, vec.y, vec.z };

		if (mesh->mTextureCoords)
		{
			aiVector3D texCoord = mesh->mTextureCoords[0][i];
			vertex.texCoord = { texCoord.x, texCoord.y };
		}
		else
			vertex.texCoord = { 0.0f, 0.0f };

		vertices.push_back(vertex);
	}

	// process triangles
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		Triangle tri = mesh->mFaces[i].mIndices;
		triangles.push_back(tri);
	}

	// do material here

	return new Mesh(model->GetFilename(), glm::mat4(1.0f), vertices, triangles);

}
