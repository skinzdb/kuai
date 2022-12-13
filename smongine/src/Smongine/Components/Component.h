#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Smongine/Renderer/Camera.h"

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

	struct Cam
	{
		Camera cam;

		Cam() = default;
	};
		
		
	
}