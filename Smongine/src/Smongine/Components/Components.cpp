#include "smpch.h"
#include "Components.h"

#include "Entity.h"
#include "Smongine/Renderer/Renderer.h"

namespace Smong {
	Transform& Component::GetTransform()
	{
		return entity->GetTransform();
	}

	void Transform::UpdateComponents()
	{	
		if (HasComponent<Camera>())
			GetComponent<Camera>().UpdateViewMatrix(pos, rot);
	}

	void Camera::UpdateViewMatrix(glm::vec3 pos, glm::vec3 rot)
	{
		viewMatrix = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(glm::quat(rot)); // Rotate then translate, aka TR
		viewMatrix = glm::inverse(viewMatrix); // Calculate inverse to get correct operation, aka (TR)^-1 = R^-1T^-1
		Renderer::SetCamera(*this, GetTransform().GetPos());
	}

	void Camera::UpdateProjectionMatrix()
	{
		if (projectionType == ProjectionType::Perspective)
		{
			projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
		}
		else
		{
			projectionMatrix = glm::ortho(-orthoSize * aspect * 0.5f, orthoSize * aspect * 0.5f, orthoNear, orthoFar);
		}
		Renderer::SetCamera(*this, GetTransform().GetPos());
	}
}