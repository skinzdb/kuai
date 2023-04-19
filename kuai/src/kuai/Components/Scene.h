#pragma once

#include "EntityComponentSystem.h"
#include "Entity.h"
#include "Components.h"

namespace kuai {
	// Forward declarations
	class RenderSystem;
	class CameraSystem;
	class LightSystem;
	
	/** \class Scene
	*	\brief Represents a game scene by holding a list of entity objects. The scene owns a main camera; the view of this camera gets rendered to the viewport.
	*	The scene owns a collection of systems that update game logic.
		@see Entity  
	*/
	class Scene
	{
	public:
		Scene();
		~Scene();

		/** 
		* Adds new entity to scene.
		*/
		std::shared_ptr<Entity> createEntity();

		/**
		* Finds an entity by its ID and returns a reference if it exists.
		*/
		std::shared_ptr<Entity> getEntityById(EntityID entity);
		std::shared_ptr<Entity> getEntityByName(const std::string& name) { KU_CORE_ASSERT(0, "Not yet implemented"); } // TODO: implement
		/**
		* Removes entity from the scene.
		*/
		void destroyEntity(EntityID entity);

		/**
		* Notify all systems of an event.
		* @param event Event that gets sent to subscribing systems.
		*/
		template<typename EventType>
		void notifySystems(EventType* event)
		{
			ECS->notifySystems(event);
		}

		/**
		* Subscribe to a callback function that gets activated whenever its event happens.
		* @param instance The instance that owns the function.
		* @param memberFn The callback function.
		*/
		template<typename T, typename EventType>
		void subscribeSystem(T* instance, void (T::* memberFn)(EventType*))
		{
			ECS->subscribeSystem(instance, memberFn);
		}

		CameraComponent& getMainCam();
		void setMainCam(CameraComponent& cam);

		Light& getMainLight();
		void setMainLight(Light& light);

		std::vector<std::shared_ptr<Entity>>::iterator begin() { return entities.begin(); }
		std::vector<std::shared_ptr<Entity>>::iterator end() { return entities.end(); }

		void update(float dt);
	private:
		std::shared_ptr<Entity> mainCam;
		std::shared_ptr<Entity> mainLight;

		EntityComponentSystem* ECS;
		std::vector<std::shared_ptr<Entity>> entities;

		std::shared_ptr<RenderSystem> renderSys;
		std::shared_ptr<CameraSystem> cameraSys;
		std::shared_ptr<LightSystem> lightSys;
	};

	class RenderSystem : public System
	{
	public:
		RenderSystem(Scene* scene);

		void onLightChanged(LightChangedEvent* event);
		void onCameraChanged(CameraChangedEvent* event);

		virtual void update(float dt);
	};

	class CameraSystem : public System
	{
	public:
		CameraSystem(Scene* scene) : System(scene) { acceptsSubset = true; }

		void setRenderSystem(std::shared_ptr<RenderSystem> renderSys) { this->renderSys = renderSys; }

		void setMainCam(CameraComponent& cam);

		virtual void update(float dt);

	private:
		std::shared_ptr<RenderSystem> renderSys;
	};

	class LightSystem : public System
	{
	public:
		LightSystem(Scene* scene) : System(scene) { acceptsSubset = true; }
		
		void setRenderSystem(std::shared_ptr<RenderSystem> renderSys) { this->renderSys = renderSys; }

		virtual void update(float dt);

	private:
		std::shared_ptr<RenderSystem> renderSys;
	};

	//class PhysicsSystem : public System
	//{
	//	virtual void update(float dt) override
	//	{
	//		for (auto& entity : entities)
	//		{
	//			Rigidbody rigidbody = entity.getComponent<Rigidbody>();

	//			entity.getTransform().pos += rigidbody.velocity * dt;
	//			
	//			if (rigidbody.useGravity)
	//			{
	//				rigidbody.velocity -= entity.getTransform().getUp() * dt; // Add gravitational force var
	//			}
	//		}
	//	}
	//};
}
