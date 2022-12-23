#pragma once

#include "smpch.h"

#include "Smongine/Events/Event.h"

namespace Smong {
	/**
	* Defines the game logic for a set of entities
	*/
	class System
	{
	public:
		//System(Scene& scene) : scene(scene) {}

		//virtual void Init() = 0;
		virtual void Update(float dt) = 0;
		//virtual void OnEvent(Event& e) = 0;

		void InsertEntity(EntityID entity)
		{
			//Entity e = *scene.GetEntityById(entity);
			entities.push_back(entity);
		}

		void RemoveEntity(EntityID entity)
		{
			//Entity e = *scene.GetEntityById(entity);
			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
		}

	protected:
		//Scene& scene;
		std::vector<EntityID> entities;
	};

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

	//class CameraSystem : public System
	//{
	//	virtual void Update(float dt) override
	//	{

	//	}
	//};
}