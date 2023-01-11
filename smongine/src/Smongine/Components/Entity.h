#pragma once

#include "EntityComponentSystem.h"
#include "Components.h"

namespace Smong {
	class EntityComponentSystem;
	/**
	* A wrapper for EntityID so one can do entity.AddComponent<T>() instead of ECS.AddComponent(entity, T)
	*/
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& other) = default;

		Entity(std::shared_ptr<EntityComponentSystem> ECS);

		Entity(std::shared_ptr<EntityComponentSystem> ECS, EntityID id); // Wrap existing ID into entity object

		template<class T, typename... Args>
		T& AddComponent(Args&&... args);

		template<class T>
		T& GetComponent();

		Transform& GetTransform();

		template<class T>
		void RemoveComponent();

		EntityID GetId() const;

		void Destroy();

	private:
		EntityID id;
		std::shared_ptr<EntityComponentSystem> ECS;

		Transform transform;
	};

	template<class T, typename ...Args>
	inline T& Entity::AddComponent(Args && ...args)
	{
		ECS->AddComponent<T>(id, std::forward<Args>(args)...);
		return ECS->GetComponent<T>(id);
	}

	template<class T>
	inline T& Entity::GetComponent()
	{
		return ECS->GetComponent<T>(id);
	}

	template<class T>
	inline void Entity::RemoveComponent()
	{
		ECS->RemoveComponent(id);
	}
}