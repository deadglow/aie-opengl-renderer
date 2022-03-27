#pragma once
#include "glm/glm.hpp"
#include "CameraShaderData.h"
#include "Transform.h"

#define DEFAULT_FOV 70.0f
#define DEFAULT_NEARPLANE 0.1f
#define DEFAULT_FARPLANE 100.0f;

class Camera
{
private:
	float nearPlane = DEFAULT_NEARPLANE;
	float farPlane = DEFAULT_FARPLANE;
	float fieldOfView = glm::radians(DEFAULT_FOV);
	float orthoSize = 10.0f;
	int layerMask = INT_MAX;

	CameraShaderData shaderData;
public:
	Transform transform;
	bool perspective = true;
	Camera();
	~Camera();

	void UpdateVPMatrix();
	const CameraShaderData GetShaderData() const;
	void Draw();
};
