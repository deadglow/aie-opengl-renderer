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
		model->Draw(csd, transform.matrix);
	}
}

void ModelInstance::AddToDrawCalls()
{
	if (overrideMaterials)
	{
		model->AddToDrawCall(transform.matrix, &materialOverrides);
	}
	else
	{
		model->AddToDrawCall(transform.matrix);
	}
}

void ModelInstance::SetMaterialOverride(int index, Material* mat)
{
	if (!overrideMaterials)
	{
		materialOverrides = model->defaultMaterials;
		overrideMaterials = true;
	}

	materialOverrides[index] = mat;
}

void ModelInstance::ClearMaterialOverrides()
{
	overrideMaterials = false;
}

Model* ModelInstance::GetBaseModel() const
{
	return model;
}

const bool ModelInstance::IsOverridingMaterials() const
{
	return overrideMaterials;
}
