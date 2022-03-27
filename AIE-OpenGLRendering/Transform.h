#pragma once
#include "glm/glm.hpp"

class Transform
{
public:
	glm::mat4 matrix = glm::mat4(1.0f);

	Transform();
	~Transform();

	glm::vec3 GetPosition() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetForward() const;

	void SetPosition(const glm::vec3 pos);
	void SetRotation(const glm::vec3 axes);

	void Translate(const glm::vec3 offset, bool worldSpace = true);

};