#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Smongine/Core/Core.h"

namespace Smong {

	struct Transform
	{
		Transform() = default;
		Transform(glm::vec3& pos) : pos(pos) {}

		glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rot = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		glm::vec3 GetUp() { return glm::rotate(glm::quat(rot), glm::vec3(0.0f, 1.0f, 0.0f)); }
		glm::vec3 GetRight() { return glm::rotate(glm::quat(rot), glm::vec3(1.0f, 0.0f, 0.0f)); }
		glm::vec3 GetForward() { return glm::rotate(glm::quat(rot), glm::vec3(0.0f, 0.0f, -1.0f)); }

		glm::mat4 GetTransform()
		{
			return glm::translate(glm::mat4(1.0f), pos) *
				glm::toMat4(glm::quat(rot)) *
				glm::scale(glm::mat4(1.0f), scale);
		}
	};

	struct Rigidbody
	{
		Rigidbody() = default;

		float mass = 1.0f;
		float drag = 0.0f;

		glm::vec3 velocity = glm::vec3(0.0f);

		bool useGravity = false;
	};

	/*struct Mesh
	{
		
	};*/

	class SMONGINE_API Camera {
	public:
		enum class ProjectionType
		{
			Perspective,
			Ortho
		};
	public:
		Camera() { UpdateProjectionMatrix(); }
		Camera(float fov, float width, float height, float zNear, float zFar) : aspect(width / height) { SetPerspective(fov, zNear, zFar); }

		void SetPerspective(float fov, float zNear, float zFar)
		{
			projectionType = ProjectionType::Perspective;
			this->fov = glm::radians(fov);
			this->zNear = zNear;
			this->zFar = zFar;
			UpdateProjectionMatrix();
		}
		void SetOrtho(float size, float zNear, float zFar)
		{
			projectionType = ProjectionType::Ortho;
			this->orthoSize = size;
			this->orthoNear = zNear;
			this->orthoFar = zFar;
			UpdateProjectionMatrix();
		}
		void SetAspect(float width, float height)
		{
			aspect = width / height;
		}

		inline glm::mat4& GetViewMatrix() { return viewMatrix; }
		inline glm::mat4& GetProjectionMatrix() { return projectionMatrix; }

	private:
		void UpdateProjectionMatrix()
		{
			if (projectionType == ProjectionType::Perspective)
			{
				projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
			}
			else
			{
				projectionMatrix = glm::ortho(-orthoSize * aspect * 0.5f, orthoSize * aspect * 0.5f, orthoNear, orthoFar);
			}
		}
		void UpdateViewMatrix(glm::vec3 pos, glm::vec3 rot)
		{
			viewMatrix = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(glm::quat(rot)); // Rotate then translate, aka TR
			viewMatrix = glm::inverse(viewMatrix); // Calculate inverse to get correct operation, aka (TR)^-1 = R^-1T^-1
		}

		ProjectionType projectionType = ProjectionType::Perspective;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		float aspect = 0.0f;

		// Perspective params
		float fov = glm::radians(60.0f);
		float zNear = 0.01f, zFar = 100.0f;

		// Ortho params
		float orthoSize = 10.0f;
		float orthoNear = -1.0f, orthoFar = 1.0f;
	};
}