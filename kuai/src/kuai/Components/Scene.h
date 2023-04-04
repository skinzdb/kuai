#pragma once

#include "EntityComponentSystem.h"
#include "Entity.h"
#include "Components.h"

namespace kuai {
	// Forward declarations
	class RenderSystem;
	class CameraSystem;
	class LightSystem;
	class AudioSystem;

	class Scene
	{
	public:
		Scene();
		~Scene();

		std::shared_ptr<Entity> createEntity();
		std::shared_ptr<Entity> getEntityById(EntityID entity);
		std::shared_ptr<Entity> getEntityByName(const std::string& name) { KU_CORE_ASSERT(0, "Not yet implemented"); } // TODO: implement
		void destroyEntity(EntityID entity);

		template<typename EventType>
		void notifySystems(EventType* event)
		{
			ECS->notifySystems(event);
		}

		template<typename T, typename EventType>
		void subscribeSystem(T* instance, void (T::* memberFn)(EventType*))
		{
			ECS->subscribeSystem(instance, memberFn);
		}

		Camera& getMainCam();
		void setMainCam(Camera& cam);

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
		std::shared_ptr<AudioSystem> audioSys;
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

		virtual void update(float dt);
	};

	class LightSystem : public System
	{
	public:
		LightSystem(Scene* scene) : System(scene) { acceptsSubset = true; }

		virtual void update(float dt);
	};

	class AudioSystem : public System
	{
	public:
		AudioSystem(Scene* scene) : System(scene) { acceptsSubset = true; }

		virtual void update(float dt);
	};

	//scene.addSystem<mask>(

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
