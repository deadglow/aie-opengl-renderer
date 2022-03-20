#include "ModelTransform.h"

ModelTransform::ModelTransform(Model* model_init)
{
	model = model_init;
}

ModelTransform::~ModelTransform()
{
}

void ModelTransform::Draw(glm::mat4 vpMatrix)
{
	if (model)
	{
		model->Draw(vpMatrix * transform);
	}
}
