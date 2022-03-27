#pragma once
#include "Model.h"
#include "CameraShaderData.h"

class ModelInstance
{
private:
	Model* model = nullptr;
	bool overrideMaterials = false;
public:
	glm::mat4 transform = glm::mat4(1.0f);
	std::vector<Material*> materialOverrides;

	ModelInstance(Model* model_init);
	~ModelInstance();

	void Draw(CameraShaderData csd);
	void AddToDrawCalls();
	void SetMaterialOverride(int index, Material* mat);
	void ClearMaterialOverrides();
};

