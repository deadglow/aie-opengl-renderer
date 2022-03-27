#include "Transform.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

Transform::Transform()
{
}

Transform::~Transform()
{
}

glm::vec3 Transform::GetPosition() const
{
	return glm::vec3(matrix[3]);
}

glm::vec3 Transform::GetRight() const
{
	return glm::vec3(matrix[0]);
}

glm::vec3 Transform::GetUp() const
{
	return glm::vec3(matrix[1]);
}

glm::vec3 Transform::GetForward() const
{
	return glm::vec3(matrix[2]);
}

void Transform::SetPosition(const glm::vec3 pos)
{
	matrix[3] = glm::vec4(pos, 1.0f);
}

void Transform::SetRotation(const glm::vec3 axes)
{
}

void Transform::Translate(const glm::vec3 offset, bool worldSpace)
{
	if (worldSpace)
	{
		glm::vec3 pos = GetPosition() + offset;
		matrix[3] = glm::vec4(pos, 1.0f);
	}
	else
	{
		matrix = matrix * glm::translate(glm::mat4(1.0f), offset);
	}
}
