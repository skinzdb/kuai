#pragma once

#include "EntityComponentSystem.h"
#include "Entity.h"
#include "Components.h"

namespace Smong {
	class LightSystem : public System
	{
	public:
		LightSystem() { acceptsSubset = true; }
		virtual void Update(float dt) {}
	};

	class RenderSystem : public System
	{
	public:
		RenderSystem() { acceptsSubset = true; }
		virtual void Update(float dt) {}
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity* CreateEntity();
		Entity* GetEntityById(EntityID entity);
		void DestroyEntity(EntityID entity);

		Camera GetMainCam();
		void SetMainCam(Camera cam);

		std::vector<EntityID> GetLights();
		std::vector<EntityID> GetRenderItems();

		std::vector<Entity*>::iterator begin() { return entities.begin(); }
		std::vector<Entity*>::iterator end() { return entities.end(); }

		void Update(float dt);
	private:
		Camera mainCam;

		EntityComponentSystem* ECS;
		std::vector<Entity*> entities;

		std::shared_ptr<LightSystem> lightSystem;	// Keeps track of all Light components
		std::shared_ptr<RenderSystem> renderSystem; // Keeps track of all MeshMaterial components
	};

	//scene.addSystem<mask>(

	//class PhysicsSystem : public System
	//{
	//	virtual void Update(float dt) override
	//	{
	//		for (auto& entity : entities)
	//		{
	//			Rigidbody rigidbody = entity.GetComponent<Rigidbody>();

	//			entity.GetTransform().pos += rigidbody.velocity * dt;
	//			
	//			if (rigidbody.useGravity)
	//			{
	//				rigidbody.velocity -= entity.GetTransform().GetUp() * dt; // Add gravitational force var
	//			}
	//		}
	//	}
	//};


}
