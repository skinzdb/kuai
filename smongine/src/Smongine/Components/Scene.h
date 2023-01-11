#pragma once

#include "EntityComponentSystem.h"
#include "Entity.h"
#include "Components.h"

namespace Smong {
	class EntityComponentSystem;
	class Entity;

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

		void GetLights();

	private:
		void Update(float dt);

		Camera mainCam;

		std::shared_ptr<EntityComponentSystem> ECS;
		std::unordered_map<EntityID, Entity*> entityMap;
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


