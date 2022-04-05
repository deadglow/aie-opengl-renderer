#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

class Transform
{
public:
	glm::mat4 matrix = glm::mat4(1.0f);

	Transform();
	~Transform();

	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetForward() const;

	void SetPosition(const glm::vec3 pos);
	void SetRotation(const glm::quat rotation);
	void SetScale(const glm::vec3 scale);


	void Translate(const glm::vec3 offset, bool worldSpace = true);
	void Rotate(const glm::quat rotation, bool worldSpace = true);

	glm::vec3 TransformPoint(const glm::vec3 point);
	glm::vec3 TransformDirection(const glm::vec3 dir);
	glm::vec3 TransformVector(const glm::vec3 vec);
	glm::vec3 InvTransformPoint(const glm::vec3 point);
	glm::vec3 InvTransformDirection(const glm::vec3 dir);
	glm::vec3 InvTransformVector(const glm::vec3 vec);
};