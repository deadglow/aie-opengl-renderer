#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "Renderer.h"

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

	shaderData.vMatrix = glm::inverse(transform.matrix);
	shaderData.ivMatrix = transform.matrix;
}

const CameraShaderData Camera::GetShaderData() const
{
	return shaderData;
}

void Camera::Draw()
{
	// clear the depth buffer and start drawing
	glClear(GL_DEPTH_BUFFER_BIT);

	UpdateVPMatrix();

	// create uniform buffer
	Renderer::SetCameraUBO(shaderData);

	std::unordered_map<Material*, std::vector<MeshDrawData>>::iterator iter;

	for (iter = Renderer::drawCalls.begin(); iter != Renderer::drawCalls.end(); iter++)
	{
		// use shader and then draw all meshes
		(*iter).first->UseShader();

		std::vector<MeshDrawData>* meshDrawDataList = &(*iter).second;
		for (int i = 0; i < meshDrawDataList->size(); ++i)
		{
			// do layermask shit here

			// draw mesh with de cam data
			meshDrawDataList->at(i).Draw(shaderData);
		}
	}

	if (clearType == CameraClearType::Skybox)
		Renderer::DrawSkybox();
	}
