#pragma once
#include "Mesh.h"
#include "CameraShaderData.h"
#include <string>
#include <vector>

class Model
{
private:
	std::string filename;
	bool loaded = false;

public:
	std::vector<Mesh*> meshes;
	std::vector<Material*> defaultMaterials;
	
	Model(const std::string filename_init);
	~Model();

	const bool IsLoaded() const;
	std::string GetFilename() const;
	
	void AddMesh(Mesh* mesh);
	void SetMaterial(const int index, Material* shaderConfig);

	void Load();
	void Unload();
	
	void Draw(CameraShaderData csd, glm::mat4 transform);
	void AddToDrawCall(glm::mat4 transform, std::vector<Material*>* materialLookup);
	void AddToDrawCall(glm::mat4 transform);
};

