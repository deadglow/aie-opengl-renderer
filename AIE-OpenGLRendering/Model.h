#pragma once
#include "Mesh.h"
#include "CameraShaderData.h"
#include <string>
#include <vector>

class Model
{
private:
	std::string filename;
	std::vector<unsigned int> meshShaderLookup;
	bool loaded = false;

public:
	std::vector<Mesh*> meshes;
	std::vector<ShaderConfiguration*> shaderConfigs;
	
	Model(const std::string filename_init);
	~Model();

	void AddMesh(Mesh* mesh);
	void AddShaderConfig(ShaderConfiguration* shaderConfig);
	void Load();
	void Unload();
	void Draw(CameraShaderData csd, glm::mat4 transform);
	void SetShaderOfMesh(const int meshIndex, const int shaderIndex);
	std::string GetFilename() const;
	const bool IsLoaded() const;
};

