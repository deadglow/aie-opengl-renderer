#include "ModelInstance.h"

ModelInstance::ModelInstance(Model* model_init)
{
	model = model_init;
}

ModelInstance::~ModelInstance()
{
}

void ModelInstance::Draw(CameraShaderData csd)
{
	if (model)
	{
		model->Draw(csd, transform);
	}
}

void ModelInstance::AddToDrawCalls()
{
	if (overrideMaterials)
	{
		model->AddToDrawCall(transform, &materialOverrides);
	}
	else
	{
		model->AddToDrawCall(transform);
	}
}

void ModelInstance::SetMaterialOverride(int index, Material* mat)
{
	if (!overrideMaterials)
	{
		materialOverrides = model->defaultMaterials;
	}

	materialOverrides[index] = mat;
}

void ModelInstance::ClearMaterialOverrides()
{
	overrideMaterials = false;
}
