#include "smpch.h"
#include "Camera.h"

#include <glm/gtx/quaternion.hpp>

namespace Smong {
	Camera::Camera(float fov, float width, float height, float zNear, float zFar) :
		fov(fov), width(width), height(height), zNear(zNear), zFar(zFar)
	{
		viewMatrix = glm::mat4(1.0f);
		UpdateViewMatrix();
		projectionMatrix = glm::perspective(
			glm::radians(fov), 
			width / height, 
			zNear, 
			zFar
		);
	}

	Camera::~Camera()
	{
	}

	inline void Camera::SetPos(float x, float y, float z)
	{
		Object3D::SetPos(x, y, z);
		UpdateViewMatrix();
	}

	inline void Camera::SetRot(float x, float y, float z)
	{
		Object3D::SetRot(x, y, z);
		UpdateViewMatrix();
	}

	glm::vec3 Camera::GetUpDir()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Camera::GetRightDir()
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 Camera::GetForwardDir()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	// View matrix: first translate by negated position, then rotate by negated rotation
	void Camera::UpdateViewMatrix()
	{
		viewMatrix = glm::translate(viewMatrix, pos) * glm::toMat4(GetOrientation()); // Rotate then translate, aka TR
		viewMatrix = glm::inverse(viewMatrix); // Calculate inverse to get correct operation, aka (TR)^-1 = R^-1T^-1
	}

}

