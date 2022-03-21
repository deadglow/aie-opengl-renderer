#include "Camera.h"
#include "glm.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/matrix_clip_space.hpp"
#include "Renderer.h"
#include "CameraShaderData.h"

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

	CameraShaderData csd;
	csd.position = transform[3];
	csd.direction = glm::normalize(transform[2]);
	csd.vpMatrix = vpMatrix;
	csd.ivpMatrix = glm::inverse(vpMatrix);
	csd.nearz = nearPlane;
	csd.farz = farPlane;

	for (int i = 0; i < Renderer::modelTransforms.size(); ++i)
	{
		ModelTransform* model = Renderer::modelTransforms[i];

		model->Draw(csd);
	}
	
}
