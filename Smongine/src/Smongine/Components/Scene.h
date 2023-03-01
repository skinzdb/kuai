#pragma once

#include "EntityComponentSystem.h"
#include "Entity.h"
#include "Components.h"

namespace Smong {
	class LightSystem;
	class RenderSystem;
	class CameraSystem;

	class Scene
	{
	public:
		Scene();
		~Scene();

		std::shared_ptr<Entity> CreateEntity();
		std::shared_ptr<Entity> GetEntityById(EntityID entity);
		void DestroyEntity(EntityID entity);

		Camera& GetMainCam();
		void SetMainCam(Camera& cam);

		std::vector<std::shared_ptr<Entity>>::iterator begin() { return entities.begin(); }
		std::vector<std::shared_ptr<Entity>>::iterator end() { return entities.end(); }

		void Update(float dt);
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

		virtual void Update(float dt);
	};

	class RenderSystem : public System
	{
	public:
		RenderSystem(Scene* scene) : System(scene) { acceptsSubset = true; }

		virtual void Update(float dt);
	};

	class CameraSystem : public System
	{
	public:
		CameraSystem(Scene* scene) : System(scene) { acceptsSubset = true; }

		virtual void Update(float dt);
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
