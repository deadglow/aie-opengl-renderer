#include "Transform.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/euler_angles.hpp"

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

glm::quat Transform::GetRotation() const
{
	glm::mat3 rotMatrix = matrix;
	return glm::toQuat(rotMatrix);
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

void Transform::SetRotation(const glm::quat rotation)
{
	glm::mat4 rotMat = glm::toMat4(rotation);
	rotMat[3] = glm::vec4(GetPosition(), 1.0f);
	
	matrix = rotMat;
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

void Transform::Rotate(const glm::quat rotation, bool worldSpace)
{
	if (worldSpace)
	{
		glm::quat finalRot = GetRotation() * rotation;
		SetRotation(finalRot);
	}
	else
	{
		// do this
	}
}

glm::vec3 Transform::TransformPoint(const glm::vec3 point)
{
	return TransformVector(point) + GetPosition();
}

glm::vec3 Transform::TransformDirection(const glm::vec3 dir)
{
	return glm::normalize(TransformVector(dir));
}

glm::vec3 Transform::TransformVector(const glm::vec3 vec)
{
	return (glm::mat3)matrix * vec;
}

glm::vec3 Transform::InvTransformPoint(const glm::vec3 point)
{
	return InvTransformVector(point) - GetPosition();
}

glm::vec3 Transform::InvTransformDirection(const glm::vec3 dir)
{
	return glm::normalize(InvTransformVector(dir));
}

glm::vec3 Transform::InvTransformVector(const glm::vec3 vec)
{
	return glm::inverse((glm::mat3)matrix) * vec;
}
