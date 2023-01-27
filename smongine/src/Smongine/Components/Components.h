#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Smongine/Core/Core.h"
#include "Smongine/Renderer/Mesh.h"
#include "Smongine/Renderer/Material.h"

namespace Smong {

	struct Transform
	{
		Transform() = default;
		Transform(glm::vec3& pos) : pos(pos) {}

		glm::vec3 GetPos() { return pos; }
		void SetPos(glm::vec3& pos) { this->pos = pos; /* TODO: Update camera view matrix */ }
		void SetPos(float x, float y, float z) { SetPos(glm::vec3(x, y, z)); }
		void Translate(glm::vec3& amount) { this->pos += amount; /* TODO: Update camera view matrix */ }
		void Translate(float x, float y, float z) { Translate(glm::vec3(x, y, z)); }

		glm::vec3 GetRot() { return rot; }
		void SetRot(glm::vec3& rot) { this->rot = glm::radians(rot); /* TODO: Update camera view matrix */ }
		void SetRot(float x, float y, float z) { Rotate(glm::vec3(x, y, z)); }
		void Rotate(glm::vec3& amount) { this->rot += glm::radians(amount); /* TODO: Update camera view matrix */ }
		void Rotate(float x, float y, float z) { Rotate(glm::vec3(x, y, z)); }

		glm::vec3 GetScale() { return scale; }
		void SetScale(glm::vec3& scale) { this->scale = scale; }
		void Scale(float factor) { this->scale *= factor; }

		glm::vec3 GetUp() { return glm::rotate(glm::quat(rot), glm::vec3(0.0f, 1.0f, 0.0f)); }
		glm::vec3 GetRight() { return glm::rotate(glm::quat(rot), glm::vec3(1.0f, 0.0f, 0.0f)); }
		glm::vec3 GetForward() { return glm::rotate(glm::quat(rot), glm::vec3(0.0f, 0.0f, -1.0f)); }

		glm::mat4 GetModelMatrix()
		{
			return glm::translate(glm::mat4(1.0f), pos) *
				glm::toMat4(glm::quat(rot)) *
				glm::scale(glm::mat4(1.0f), scale);
		}
	private:
		glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rot = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };	
	};

	struct Rigidbody
	{
		Rigidbody() = default;

		float mass = 1.0f;
		float drag = 0.0f;

		glm::vec3 velocity = { 0.0f, 0.0f, 0.0f };

		bool useGravity = false;
	};

	struct MeshMaterial
	{
		MeshMaterial() = default;

		MeshMaterial(Mesh& mesh, Material& material) : mesh(mesh), material(material)
		{
		}

		void Render()
		{
			material.Render(); // Render first to bind textures
			mesh.Render();
		}

		Mesh& mesh;
		Material& material;
	};

	struct Light 
	{
		enum LightType
		{
			Directional = 0,
			Point = 1,
			Spot = 2
		};

		Light() = default;
		Light(float intensity) : intensity(intensity), type(LightType::Directional) {}
		Light(float intensity, float linear, float quadratic) : intensity(intensity), linear(linear), quadratic(quadratic), type(LightType::Point) {}

		LightType type = LightType::Point;

		glm::vec3 col = { 1.0f, 1.0f, 1.0f };
		float intensity = 1;

		// Only used for point light and spot light (attenuation values)
		float linear = 1;	
		float quadratic = 1;

		// Only used for spot light
		float angle = 30;
	};

	struct Camera
	{
		enum class ProjectionType
		{
			Perspective,
			Ortho
		};

		Camera() = default;

		Camera(float fov, float width, float height, float zNear, float zFar) : aspect(width / height) 
		{ 
			SetPerspective(fov, zNear, zFar); 
			UpdateViewMatrix(glm::vec3(0.0f), glm::vec3(0.0f));
		}

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

		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::mat4 projectionMatrix = glm::mat4(1.0f);

		float aspect = 1.0f;

		// Perspective params
		float fov = glm::radians(60.0f);
		float zNear = 0.01f, zFar = 100.0f;

		// Ortho params
		float orthoSize = 10.0f;
		float orthoNear = -1.0f, orthoFar = 1.0f;

		friend class CameraTransformSystem;
	};
}