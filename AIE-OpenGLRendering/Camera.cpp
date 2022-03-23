#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "Renderer.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::UpdateVPMatrix()
{
	if (perspective)
	{
		shaderData.pMatrix = glm::perspective(fieldOfView, Renderer::GetAspect(), nearPlane, farPlane);
	}
	else
	{
		float halfSize = orthoSize / 2;
		float height = Renderer::GetAspect() * halfSize;
		shaderData.pMatrix = glm::ortho(-halfSize, halfSize, -halfSize, halfSize, nearPlane, farPlane);
	}

	shaderData.ipMatrix = glm::inverse(shaderData.pMatrix);

	shaderData.vMatrix = glm::inverse(transform);
	shaderData.ivMatrix = transform;
}

const CameraShaderData Camera::GetShaderData() const
{
	return shaderData;
}

void Camera::Draw()
{
	UpdateVPMatrix();

	// create uniform buffer
	Renderer::SetCameraUBO(shaderData);

	for (int i = 0; i < Renderer::modelTransforms.size(); ++i)
	{
		ModelTransform* model = Renderer::modelTransforms[i];

		model->Draw(shaderData);
	}
	
}
