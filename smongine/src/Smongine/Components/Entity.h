#pragma once

#include "smpch.h"

#include "EntityComponentSystem.h"
#include "Component.h"

namespace Smong {
	/**
	* A wrapper for EntityID so one can do entity.AddComponent<T>() instead of ECS.AddComponent(entity, T)
	*/
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& other) = default;

		Entity(EntityComponentSystem& ECS) : 
			ECS(ECS),
			id(ECS.CreateEntity())
		{
			AddComponent<Transform>();
		}

		template<class T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ECS.AddComponent(id, std::forward<Args>(args)...);
		}

		template<class T>
		T& GetComponent()
		{
			return ECS.GetComponent<T>(id);
		}

		template<class T>
		void RemoveComponent()
		{
			ECS.RemoveComponent(id);
		}

		EntityID GetID() const
		{
			return id;
		}

		void Destroy()
		{
			ECS.DestroyEntity(id);
		}

	private:
		EntityID id;
		EntityComponentSystem& ECS;
	};
}