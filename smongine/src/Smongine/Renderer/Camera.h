#pragma once

#include "Object3D.h"

#include <glm/glm.hpp>

namespace Smong {
	class Camera : Object3D {
	public:
		Camera(float fov, float width, float height, float zNear, float zFar);
		~Camera();

		inline virtual void SetPos(float x, float y, float z) override;
		inline virtual void SetRot(float x, float y, float z) override;

		glm::vec3 GetUpDir();
		glm::vec3 GetRightDir();
		glm::vec3 GetForwardDir();

		glm::mat4& GetViewMatrix() { return viewMatrix; }
		glm::mat4& GetProjectionMatrix() { return projectionMatrix; }

	private:
		void UpdateViewMatrix();

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		float fov;
		float width, height;
		float zNear, zFar;
	};
}