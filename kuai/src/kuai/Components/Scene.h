#pragma once

#include "EntityComponentSystem.h"
#include "Entity.h"
#include "Components.h"

namespace kuai {
	class LightSystem;
	class RenderSystem;

	class Scene
	{
	public:
		Scene();
		~Scene();

		std::shared_ptr<Entity> createEntity();
		std::shared_ptr<Entity> getEntityById(EntityID entity);
		void destroyEntity(EntityID entity);

		Camera& getMainCam();
		void setMainCam(Camera& cam);

		std::vector<std::shared_ptr<Entity>>::iterator begin() { return entities.begin(); }
		std::vector<std::shared_ptr<Entity>>::iterator end() { return entities.end(); }

		void update(float dt);
	private:
		std::shared_ptr<Entity> mainCam;

		EntityComponentSystem* ECS;
		std::vector<std::shared_ptr<Entity>> entities;

		std::shared_ptr<LightSystem> lightSys;
		std::shared_ptr<RenderSystem> renderSys;
	};

	class LightSystem : public System
	{
	public:
		LightSystem(Scene* scene) : System(scene) { acceptsSubset = true; }

		virtual void update(float dt);
	};

	class RenderSystem : public System
	{
	public:
		RenderSystem(Scene* scene) : System(scene) { acceptsSubset = true; }

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
