#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Smong {
	class Object3D {
	public:
		Object3D() = default;
		~Object3D() = default;

		inline virtual glm::vec3& GetPosition() { return pos; }
		inline virtual void SetPos(float x, float y, float z) { pos = glm::vec3(x, y, z); }

		inline virtual glm::vec3& GetRot() { return rot; }
		inline virtual glm::quat& GetOrientation() { return glm::quat(rot); }
		inline virtual void SetRot(float x, float y, float z) { rot = glm::vec3(x, y, z); }

		inline virtual glm::vec3& GetScale() { return scale; }
		inline virtual void SetScale(float x, float y, float z) { scale = glm::vec3(x, y, z); }

		inline void Translate(float dx, float dy, float dz) { SetPos(pos.x + dx, pos.y + dy, pos.z + dz); }
		inline void Rotate(float dx, float dy, float dz) { SetRot(rot.x + dx, rot.y + dy, rot.z + dz); }

	protected:
		glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rot = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	};
}