#pragma once

#include "EntityComponentSystem.h"
#include "Components.h"

namespace Smong {
	/**
	* A wrapper for EntityID so one can do entity.AddComponent<T>() instead of ECS.AddComponent(entity, T)
	*/
	class Entity
	{
	public:
		Entity(EntityComponentSystem* ECS) : ECS(ECS)
		{
			id = ECS->CreateEntity();
			AddComponent<Transform>(); // Every entity has a transform
		}
		Entity(EntityComponentSystem* ECS, EntityID id) : ECS(ECS), id(id) // Wrap entity object over existing ID
		{
		}

		template<class T, typename ...Args>
		T& AddComponent(Args&& ...args)
		{
			ECS->AddComponent<T>(id, std::forward<Args>(args)...);
			return ECS->GetComponent<T>(id);
		}

		template<class T>
		T& GetComponent()
		{	
			return ECS->GetComponent<T>(id);
		}

		template<class T>
		bool HasComponent()
		{
			return ECS->HasComponent<T>(id);
		}

		template<class T>
		void RemoveComponent()
		{
			ECS->RemoveComponent(id);
		}

		Transform& GetTransform()
		{
			return ECS->GetComponent<Transform>(id);
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
		EntityComponentSystem* ECS;
	};

}