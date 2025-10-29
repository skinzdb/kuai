#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "rekuai/Renderer/Mesh.h"
#include "rekuai/Renderer/Material.h"

#include "EntityManager.h"

namespace kuai {
	struct Parent
	{
		EntityId parent = null_entity;
	};

	struct Child
	{
		std::vector<EntityId> children;
	};

	struct Transform
	{
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 rot = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		glm::vec3 up() { return glm::rotate(glm::quat(rot), glm::vec3(0.0f, 1.0f, 0.0f)); }
		glm::vec3 right() { return glm::rotate(glm::quat(rot), glm::vec3(1.0f, 0.0f, 0.0f)); }
		glm::vec3 forward() { return glm::rotate(glm::quat(rot), glm::vec3(0.0f, 0.0f, -1.0f)); }

		glm::mat4 get_model_matrix() const { return model_matrix; }

		bool operator==(const Transform& other) const
		{
			return pos == other.pos && rot == other.rot && scale == other.scale;
		}

		void calc_model_matrix()
		{
			model_matrix = glm::translate(glm::mat4(1.0f), pos) *
				glm::toMat4(glm::quat(rot)) *
				glm::scale(glm::mat4(1.0f), scale);
		}

	private:
		glm::mat4 model_matrix;
	};

	struct MeshRenderer
	{
		MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) : mesh(mesh), material(material) {}

		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;
	};

	struct Camera
	{
		enum class ProjectionType
		{
			Perspective,
			Ortho
		};

		Camera(float fov, float width, float height, float z_near, float z_far) :
			proj_type(ProjectionType::Perspective),	fov(fov), aspect(width / height), z_near(z_near), z_far(z_far)
		{
			update_view_matrix(glm::vec3(), glm::vec3());
			update_proj_matrix();
		}

		float aspect;

		float fov;	// For perspective
		float size; // For ortho

		float z_near;
		float z_far;

	// private:
		void update_view_matrix(const glm::vec3& pos, const glm::vec3& rot)
		{
			view_matrix = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(glm::quat(rot)); // Rotate then translate, aka TR
			view_matrix = glm::inverse(view_matrix); // Calculate inverse to get correct operation, aka (TR)^-1 = R^-1T^-1
		}

		void update_proj_matrix()
		{
			if (proj_type == ProjectionType::Perspective)
			{
				proj_matrix = glm::perspective((float)fov, aspect, z_near, z_far);
			}
			else
			{
				proj_matrix = glm::ortho(
					-size * aspect * 0.5f, size * aspect * 0.5f,
					-size * 0.5f, size * 0.5f,
					z_near, z_far
				);
			}
		}

	private:
		ProjectionType proj_type;

		glm::mat4 view_matrix = glm::mat4();
		glm::mat4 proj_matrix = glm::mat4();

		friend class Renderer;
	};

}
