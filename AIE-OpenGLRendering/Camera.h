#pragma once
#include "glm.hpp"

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

	glm::mat4 vpMatrix = glm::mat4(1.0f);
public:
	bool perspective = true;
	glm::mat4 transform = glm::mat4(1.0f);
	Camera();
	~Camera();

	void UpdateVPMatrix();
	glm::mat4 GetVPMatrix() const;
	void Draw();
};

