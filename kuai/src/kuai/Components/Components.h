#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "kuai/Core/Core.h"
#include "kuai/Renderer/Mesh.h"
#include "kuai/Renderer/Model.h"
#include "kuai/Renderer/Material.h"
#include "kuai/Sound/AudioClip.h"
#include "kuai/Events/Event.h"

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

		bool changed = false;

	private:
		Entity* entity = nullptr;
	};

	// Forward Declarations
	class Camera;

	class Transform : public Component
	{
	public:
		Transform(Entity* entity) : Component(entity) {}

		Transform(Entity* entity, glm::vec3& pos) : Component(entity), pos(pos) {}

		glm::vec3 getPos() { return pos; }
		void setPos(const glm::vec3& pos) 
		{ 
			this->pos = pos; 
			updateComponents(); 
			calcModelMatrix(); 
		}
		void setPos(float x, float y, float z) { setPos({ x, y, z }); }

		void translate(const glm::vec3& amount) 
		{ 
			this->pos += amount; 
			updateComponents(); 
			calcModelMatrix(); 
		}
		void translate(float x, float y, float z) { translate({ x, y, z }); }

		glm::vec3 getRot() { return glm::degrees(rot); }
		void setRot(const glm::vec3& rot) 
		{ 
			this->rot = glm::radians(rot); 
			updateComponents(); 
			calcModelMatrix(); 
		}
		void setRot(float x, float y, float z) { setRot({ x, y, z }); }

		void rotate(const glm::vec3& amount) 
		{ 
			this->rot += glm::radians(amount);
			updateComponents(); 
			calcModelMatrix();
		}
		void rotate(float x, float y, float z) { rotate({ x, y, z }); }

		glm::vec3 getScale() { return scale; }
		void setScale(const glm::vec3& scale)
		{ 
			this->scale = scale; 
			calcModelMatrix(); 
		}
		void setScale(float x, float y, float z) { setScale({ x, y, z }); }

		glm::vec3 getUp() { return glm::rotate(glm::quat(rot), glm::vec3(0.0f, 1.0f, 0.0f)); }
		glm::vec3 getRight() { return glm::rotate(glm::quat(rot), glm::vec3(1.0f, 0.0f, 0.0f)); }
		glm::vec3 getForward() { return glm::rotate(glm::quat(rot), glm::vec3(0.0f, 0.0f, -1.0f)); }

		glm::mat4 getModelMatrix() { return modelMatrix; }

	private:
		void updateComponents();

		void calcModelMatrix()
		{
			modelMatrix = glm::translate(glm::mat4(1.0f), pos) *
				glm::toMat4(glm::quat(rot)) *
				glm::scale(glm::mat4(1.0f), scale);
		}

		glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rot = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 modelMatrix = glm::mat4(1.0f);
	};

	class Rigidbody : public Component
	{
	public:
		Rigidbody(Entity* entity) : Component(entity) {};

		float mass = 1.0f;
		float drag = 0.0f;

		glm::vec3 velocity = { 0.0f, 0.0f, 0.0f };

		bool useGravity = false;
	};

	class Rigidbody2D : public Component
	{
	public:
		Rigidbody2D(Entity* entity) : Component(entity) {};

		bool fixedRotation = false;

		float mass = 1.0f;
		float drag = 0.0f;

		glm::vec3 velocity = { 0.0f, 0.0f, 0.0f };

		bool useGravity = false;
	};

	class BoxCollider2D : public Component
	{
	public:
		BoxCollider2D(Entity* entity) : Component(entity) {}

		glm::vec2 getSize() { return size; }
		void setSize(float x, float y) { size = glm::vec2(x, y); }

		glm::vec2 getOffset() { return offset; }
		void setOffset(float x, float y) { offset = glm::vec2(x, y); }

		float getRestitution() { return restitution; }
		void setSize(float restitution) { this->restitution = restitution; }

		float getFriction() { return friction; }
		void setFriction(float friction) { this->friction = friction; }

	private:
		glm::vec2 size = { 0.5f, 0.5f };
		glm::vec2 offset = { 0.0f, 0.0f };

		// TODO: should be properties of a physics material
		float restitution = 0.5f;
		float friction = 0.5f;
	};

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer(Entity* entity) : Component(entity) {}

		MeshRenderer(Entity* entity, std::shared_ptr<Model> model) : Component(entity), model(model) {}

		MeshRenderer(Entity* entity, std::shared_ptr<Mesh> mesh) : Component(entity), model(std::make_shared<Model>(mesh)) {}

		void render()
		{
			if (model)
				model->render();
		}

		std::shared_ptr<Model> getModel() { return model; }
		void setModel(std::shared_ptr<Model> model) { this->model = model; }

	private:
		std::shared_ptr<Model> model;
		
		bool castShadows = false;
	};

	class LightCounter
	{
	public:
		static uint32_t lightCount;
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

		Light(Entity* entity, LightType type, float intensity, float linear, float quadratic, float angle)
			: Component(entity), intensity(intensity), linear(linear), quadratic(quadratic), angle(angle), type(type) 
		{
			lightId = LightCounter::lightCount++;
			changed = true;
		}

		Light(Entity* entity, float intensity) 
			: Light(entity, LightType::Directional, intensity, 0.2f, 0.2f, 30.0f) {}

		Light(Entity* entity, float intensity, float linear, float quadratic)
			: Light(entity, LightType::Point, intensity, linear, quadratic, 30.0f) {}

		Light(Entity* entity) : Light(entity, LightType::Point, 1.0f, 0.2f, 0.2f, 30.0f) {}

		LightType getType() { return type; }
		void setType(LightType type) { this->type = type; changed = true; }

		glm::vec3 getCol() { return col; }
		void setCol(const glm::vec3& col) 
		{ 
			this->col = col; 
			changed = true;
		}
		void setCol(float x, float y, float z) { setCol({ x, y, z }); }

		float getIntensity() { return intensity; }
		void setIntensity(float intensity) { this->intensity = intensity; changed = true; }

		float getLinear() { return linear; }
		float getQuadratic() { return quadratic; }
		void setAttenuation(float linear, float quadratic) { this->linear = linear; this->quadratic = quadratic; changed = true; }

		float getAngle() { return angle; }
		void setAngle(float angle) { this->angle = angle; changed = true; }

		uint32_t getId() { return lightId; }

	private:
		LightType type = LightType::Point;

		glm::vec3 col = { 1.0f, 1.0f, 1.0f };
		float intensity = 1;

		// Only used for point light and spot light (attenuation values)
		float linear = 0.2f;	
		float quadratic = 0.2f;

		// Only used for spot light
		float angle = 30;

		uint32_t lightId = 0;
	};

	class LightChangedEvent : public Event
	{
	public:
		LightChangedEvent(Light* light) : light(light) {}

		EVENT_CLASS_TYPE(EventType::LightChanged);
		EVENT_CLASS_CATEGORY(0);

		Light* light;
	};
	
	class Camera : public Component
	{
	public:
		enum class ProjectionType
		{
			Perspective,
			Ortho
		};

		Camera(Entity* entity) : Component(entity) { changed = true; }

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

	struct CameraChangedEvent : public Event
	{
		CameraChangedEvent(Camera* cam) : cam(cam) {}

		EVENT_CLASS_TYPE(EventType::CameraChanged);
		EVENT_CLASS_CATEGORY(0);

		Camera* cam;
	};

	// Forward declarations
	class Listener;
	class AudioSource;

	class AudioListener : public Component
	{
	public:
		AudioListener(Entity* entity);

	private:
		void update();

		friend class Transform;
	};

	class AudioSourceComponent : public Component
	{
	public:
		AudioSourceComponent(Entity* entity, bool stream = false);

		AudioSource* source;

	private:
		void update();

		friend class Transform;
	};
}

#include "Entity.h"

namespace kuai
{
	template<typename T>
	bool Component::hasComponent() { return entity->template hasComponent<T>(); }

	template<typename T>
	T& Component::getComponent() { return entity->template getComponent<T>(); }
}
