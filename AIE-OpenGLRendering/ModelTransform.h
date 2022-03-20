#pragma once
#include "Model.h"

class ModelTransform
{
private:
	Model* model = nullptr;
public:
	glm::mat4 transform = glm::mat4(1.0f);

	ModelTransform(Model* model_init);
	~ModelTransform();

	void Draw(glm::mat4 vpMatrix);
};

