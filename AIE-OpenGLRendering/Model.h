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

	const bool IsLoaded() const;
	std::string GetFilename() const;
	
	void AddMesh(Mesh* mesh);
	void AddShaderConfig(ShaderConfiguration* shaderConfig);
	void SetShaderOfMesh(const int meshIndex, const int shaderIndex);

	void Load();
	void Unload();
	
	void Draw(CameraShaderData csd, glm::mat4 transform);
};

