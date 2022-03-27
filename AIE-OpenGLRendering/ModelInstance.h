#pragma once
#include "Model.h"
#include "CameraShaderData.h"
#include "Transform.h"

class ModelInstance
{
private:
	Model* model = nullptr;
	bool overrideMaterials = false;
public:
	Transform transform;
	std::vector<Material*> materialOverrides;

	ModelInstance(Model* model_init);
	~ModelInstance();

	void Draw(CameraShaderData csd);
	void AddToDrawCalls();
	void SetMaterialOverride(int index, Material* mat);
	void ClearMaterialOverrides();

	Model* GetBaseModel() const;
};

