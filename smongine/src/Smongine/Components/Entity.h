#pragma once

#include "smpch.h"

#include "EntityComponentSystem.h"
#include "Components.h"

namespace Smong {
	/**
	* A wrapper for EntityID so one can do entity.AddComponent<T>() instead of ECS.AddComponent(entity, T)
	*/
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& other) = default;

		Entity(std::shared_ptr<EntityComponentSystem> ECS)	
		{
			this->ECS = ECS;
			id = ECS->CreateEntity();
			transform = AddComponent<Transform>(); // Every base object will have a transform component
		}

		template<class T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ECS->AddComponent<T>(id, std::forward<Args>(args)...);
			return ECS->GetComponent<T>(id);
		}

		template<class T>
		T& GetComponent()
		{
			return ECS->GetComponent<T>(id);
		}

		Transform& GetTransform()
		{
			return transform;
		}

		template<class T>
		void RemoveComponent()
		{
			ECS->RemoveComponent(id);
		}

		EntityID GetId() const
		{
			return id;
		}

		void Destroy()
		{
			ECS->DestroyEntity(id);
		}

	private:
		EntityID id;
		std::shared_ptr<EntityComponentSystem> ECS;

		Transform transform;
	};
}