#pragma once

#include "smpch.h"

#include "Entity.h"
#include "Scene.h"
#include "Smongine/Events/Event.h"

namespace Smong {
	/**
	* Defines the game logic for a set of entities
	*/
	class System
	{
	public:
		System(Scene& scene) : scene(scene) {}

		//virtual void Init() = 0;
		virtual void Update(float dt) = 0;
		//virtual void OnEvent(Event& e) = 0;

		void InsertEntity(EntityID entity)
		{
			Entity e = *scene.GetEntityById(entity);
			entities.push_back(e);
		}

		void RemoveEntity(EntityID entity)
		{
			Entity e = *scene.GetEntityById(entity);
			entities.erase(std::remove(entities.begin(), entities.end(), e), entities.end());
		}

	protected:
		Scene& scene;
		std::vector<Entity> entities;
	};

	class PhysicsSystem : public System
	{
		virtual void Update(float dt) override
		{
			for (auto& entity : entities)
			{
				Rigidbody rigidbody = entity.GetComponent<Rigidbody>();

				entity.GetTransform().pos += rigidbody.velocity * dt;
				
				if (rigidbody.useGravity)
				{
					rigidbody.velocity -= entity.GetTransform().GetUp() * dt; // Add gravitational force var
				}
			}
		}
	};

	class CameraSystem : public System
	{
		virtual void Update(float dt) override
		{

		}
	};
}