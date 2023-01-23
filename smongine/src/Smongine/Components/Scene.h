#pragma once

#include "EntityComponentSystem.h"
#include "Entity.h"
#include "Components.h"

namespace Smong {
	class LightSystem : public System
	{
	public:
		LightSystem() { acceptsSubset = true; };
		virtual void Update(float dt) {};
	};

	class Scene
	{
		using EntityMap = std::unordered_map<EntityID, Entity*>;
	public:
		Scene();
		~Scene();

		Entity* CreateEntity();
		Entity* GetEntityById(EntityID entity);
		void DestroyEntity(EntityID entity);

		Camera GetMainCam();
		void SetMainCam(Camera cam);

		std::vector<EntityID> GetLights();

		EntityMap::iterator begin() { return entityMap.begin(); }
		EntityMap::iterator end() { return entityMap.end(); }

		void Update(float dt);
	private:
		Camera mainCam;

		EntityComponentSystem* ECS;
		EntityMap entityMap;

		std::shared_ptr<LightSystem> lightSystem;
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

	class MeshMaterialSystem : public System
	{
	public:
		virtual void Update(float dt) {};

	private:

	};
}


