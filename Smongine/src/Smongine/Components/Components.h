#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Smongine/Core/Core.h"
#include "Smongine/Renderer/Mesh.h"
#include "Smongine/Renderer/Material.h"

namespace Smong {
	// Forward Declarations
	class Entity;
	class Transform;

	/**
	* Base class for all components
	*/
	class Component
	{
	public:
		Component(Entity* entity) : entity(entity) {}

		template<typename T>
		bool HasComponent();

		template<typename T>
		T& GetComponent();

		Transform& GetTransform();

	private:
		Entity* entity;
	};

	template<typename T>
	inline bool Component::HasComponent() { return entity->HasComponent<T>(); }

	template<typename T>
	inline T& Component::GetComponent() { return entity->GetComponent<T>(); }

	// Forward Declarations
	class Camera;

	class Transform : public Component
	{
	public:
		Transform(Entity* entity) : Component(entity) {}

		Transform(Entity* entity, glm::vec3& pos) : Component(entity), pos(pos) {}

		glm::vec3 GetPos() { return pos; }
		void SetPos(glm::vec3& pos) { this->pos = pos; UpdateComponents(); }
		void SetPos(float x, float y, float z) { SetPos(glm::vec3(x, y, z)); }

		void Translate(glm::vec3& amount) { this->pos += amount; UpdateComponents(); }
		void Translate(float x, float y, float z) { Translate(glm::vec3(x, y, z)); }

		glm::vec3 GetRot() { return rot; }
		void SetRot(glm::vec3& rot) { this->rot = glm::radians(rot); UpdateComponents(); }
		void SetRot(float x, float y, float z) { Rotate(glm::vec3(x, y, z)); }

		void Rotate(glm::vec3& amount) { this->rot += glm::radians(amount); UpdateComponents(); }
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
		void UpdateComponents();

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

	class MeshMaterial : public Component
	{
	public:
		MeshMaterial(Entity* entity) : Component(entity) {}
	
		MeshMaterial(Entity* entity, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) 
			: Component(entity), mesh(mesh), material(material) {}


		void Render()
		{
			material->Render(); // Render first to bind textures
			mesh->Render();
		}

		std::shared_ptr<Mesh> mesh = nullptr;
		std::shared_ptr<Material> material = nullptr;
	};

	class Light : public Component
	{
	public:
		enum class LightType
		{
			Directional = 0,
			Point = 1,
			Spot = 2
		};

		Light(Entity* entity) : Component(entity) {}

		Light(Entity* entity, float intensity) 
			: Component(entity), intensity(intensity), type(LightType::Directional) {}

		Light(Entity* entity, float intensity, float linear, float quadratic)
			: Component(entity), intensity(intensity), linear(linear), quadratic(quadratic), type(LightType::Point) {}

		Light(Entity* entity, float intensity, float linear, float quadratic, float angle)
			: Component(entity), intensity(intensity), linear(linear), quadratic(quadratic), angle(angle), type(LightType::Spot) {}

		LightType GetType() { return type; }
		void SetType(LightType type) { this->type = type; }

		glm::vec3 GetCol() { return col; }
		void SetCol(glm::vec3 col) { this->col = col; }

		float GetIntensity() { return intensity; }
		void SetIntensity(float intensity) { this->intensity = intensity; }

		float GetLinear() { return linear; }
		float GetQuadratic() { return quadratic; }
		void SetAttenuation(float linear, float quadratic) { this->linear = linear; this->quadratic = quadratic; }

		float GetAngle() { return angle; }
		void SetAngle(float angle) { this->angle = angle; }

	private:
		LightType type = LightType::Point;

		glm::vec3 col = { 1.0f, 1.0f, 1.0f };
		float intensity = 1;

		// Only used for point light and spot light (attenuation values)
		float linear = 1;	
		float quadratic = 1;

		// Only used for spot light
		float angle = 30;
	};

	class Camera : public Component
	{
	public:
		enum class ProjectionType
		{
			Perspective,
			Ortho
		};

		Camera(Entity* entity) : Component(entity) {}

		Camera(Entity* entity, float fov, float width, float height, float zNear, float zFar) 
			: Component(entity), aspect(width / height) 
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

		inline glm::mat4& GetViewMatrix()  { return viewMatrix; }
		inline glm::mat4& GetProjectionMatrix() { return projectionMatrix; }

		void UpdateViewMatrix(glm::vec3 pos, glm::vec3 rot);

	private:
		void UpdateProjectionMatrix();

	private:
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
	};
}
