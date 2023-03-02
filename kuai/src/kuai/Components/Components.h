#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "kuai/Core/Core.h"
#include "kuai/Renderer/Mesh.h"
#include "kuai/Renderer/Material.h"

namespace kuai {
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
		bool hasComponent();

		template<typename T>
		T& getComponent();

		Transform& getTransform();

	private:
		Entity* entity;
	};

	template<typename T>
	inline bool Component::hasComponent() { return entity->hasComponent<T>(); }

	template<typename T>
	inline T& Component::getComponent() { return entity->getComponent<T>(); }

	// Forward Declarations
	class Camera;

	class Transform : public Component
	{
	public:
		Transform(Entity* entity) : Component(entity) {}

		Transform(Entity* entity, glm::vec3& pos) : Component(entity), pos(pos) {}

		glm::vec3 getPos() { return pos; }
		void setPos(glm::vec3& pos) { this->pos = pos; updateComponents(); }
		void setPos(float x, float y, float z) { setPos(glm::vec3(x, y, z)); }

		void translate(glm::vec3& amount) { this->pos += amount; updateComponents(); }
		void translate(float x, float y, float z) { translate(glm::vec3(x, y, z)); }

		glm::vec3 getRot() { return rot; }
		void setRot(glm::vec3& rot) { this->rot = glm::radians(rot); updateComponents(); }
		void setRot(float x, float y, float z) { rotate(glm::vec3(x, y, z)); }

		void rotate(glm::vec3& amount) { this->rot += glm::radians(amount); updateComponents(); }
		void rotate(float x, float y, float z) { rotate(glm::vec3(x, y, z)); }

		glm::vec3 getScale() { return scale; }
		void setScale(glm::vec3& scale) { this->scale = scale; }
		void Scale(float factor) { this->scale *= factor; }

		glm::vec3 getUp() { return glm::rotate(glm::quat(rot), glm::vec3(0.0f, 1.0f, 0.0f)); }
		glm::vec3 getRight() { return glm::rotate(glm::quat(rot), glm::vec3(1.0f, 0.0f, 0.0f)); }
		glm::vec3 getForward() { return glm::rotate(glm::quat(rot), glm::vec3(0.0f, 0.0f, -1.0f)); }

		glm::mat4 getModelMatrix()
		{
			return glm::translate(glm::mat4(1.0f), pos) *
				glm::toMat4(glm::quat(rot)) *
				glm::scale(glm::mat4(1.0f), scale);
		}

	private:
		void updateComponents();

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
			material->render(); // Render first to bind textures
			mesh->render();
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

		LightType getType() { return type; }
		void setType(LightType type) { this->type = type; }

		glm::vec3 getCol() { return col; }
		void setCol(glm::vec3 col) { this->col = col; }

		float getIntensity() { return intensity; }
		void setIntensity(float intensity) { this->intensity = intensity; }

		float getLinear() { return linear; }
		float getQuadratic() { return quadratic; }
		void setAttenuation(float linear, float quadratic) { this->linear = linear; this->quadratic = quadratic; }

		float getAngle() { return angle; }
		void setAngle(float angle) { this->angle = angle; }

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
			setPerspective(fov, zNear, zFar); 
			updateViewMatrix(glm::vec3(0.0f), glm::vec3(0.0f));
		}

		void setPerspective(float fov, float zNear, float zFar)
		{
			projectionType = ProjectionType::Perspective;
			this->fov = glm::radians(fov);
			this->zNear = zNear;
			this->zFar = zFar;
			updateProjectionMatrix();
		}
		void setOrtho(float size, float zNear, float zFar)
		{
			projectionType = ProjectionType::Ortho;
			this->orthoSize = size;
			this->orthoNear = zNear;
			this->orthoFar = zFar;
			updateProjectionMatrix();
		}
		void setAspect(float width, float height)
		{
			aspect = width / height;
		}

		inline glm::mat4& getViewMatrix()  { return viewMatrix; }
		inline glm::mat4& getProjectionMatrix() { return projectionMatrix; }

		void updateViewMatrix(glm::vec3 pos, glm::vec3 rot);

	private:
		void updateProjectionMatrix();

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

	class AudioSource : public Component
	{
	public:
		AudioSource(Entity* entity);

		void play();
		void stop();

		float getGain() { return gain; }
		void setGain(float gain) { this->gain = gain; }

		float getPitch() { return pitch; }
		void setPitch(float pitch) { this->pitch = pitch; }

		void setLoop(bool loop) { this->loop = loop; }
		bool isLoop() { return loop; }

	private:
		float gain = 1.0f;
		float pitch = 1.0f;

		bool loop = false;

		uint32_t sourceId = 0;
	};
}