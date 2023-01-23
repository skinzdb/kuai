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
		Entity(const Entity& other) = default;

		Entity(EntityComponentSystem* ECS)
		{
			this->ECS = ECS;
			id = ECS->CreateEntity();
			transform = AddComponent<Transform>(); // Every base object will have a transform component
		}
		Entity(EntityComponentSystem* ECS, EntityID id) // Wrap existing ID into entity object
		{
			this->ECS = ECS;
			this->id = id;
			transform = GetComponent<Transform>();
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
			return transform;
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

		Transform transform;
	};

}