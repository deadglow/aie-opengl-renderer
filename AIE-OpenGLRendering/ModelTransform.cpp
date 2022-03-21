#include "ModelTransform.h"

ModelTransform::ModelTransform(Model* model_init)
{
	model = model_init;
}

ModelTransform::~ModelTransform()
{
}

void ModelTransform::Draw(CameraShaderData csd)
{
	if (model)
	{
		model->Draw(csd, transform);
	}
}
