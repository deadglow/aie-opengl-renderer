#include "Camera.h"
#include "glm.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/matrix_clip_space.hpp"
#include "Renderer.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::UpdateVPMatrix()
{
	glm::mat4 projection;

	if (perspective)
	{
		projection = glm::perspective(fieldOfView, Renderer::GetAspect(), nearPlane, farPlane);
	}
	else
	{
		float halfSize = orthoSize / 2;
		float height = Renderer::GetAspect() * halfSize;
		projection = glm::ortho(-halfSize, halfSize, -halfSize, halfSize, nearPlane, farPlane);
	}

	vpMatrix = projection * glm::inverse(transform);
}

glm::mat4 Camera::GetVPMatrix() const
{
	return vpMatrix;
}

void Camera::Draw()
{
	UpdateVPMatrix();

	for (int i = 0; i < Renderer::modelTransforms.size(); ++i)
	{
		ModelTransform* model = Renderer::modelTransforms[i];

		model->Draw(vpMatrix);
	}
	
}
