#pragma once

#include "EntityComponentSystem.h"
#include "Components.h"

namespace kuai {
	/**
	* A wrapper for EntityID so one can do entity.AddComponent<T>() instead of ECS.addComponent(entity, T)
	*/
	class Entity
	{
	public:
		Entity(EntityComponentSystem* ECS) : ECS(ECS)
		{
			id = ECS->createEntity();
			addComponent<Transform>(); // Every entity has a transform
		}
		Entity(EntityComponentSystem* ECS, EntityID id) : ECS(ECS), id(id) // Wrap entity object over existing ID
		{
		}

		template<class T, typename ...Args>
		T& addComponent(Args&& ...args)
		{
			// Send a reference of this entity to the component
			ECS->addComponent<T>(id, this, std::forward<Args>(args)...);
			return ECS->getComponent<T>(id);
		}

		template<class T>
		T& getComponent()
		{	
			return ECS->getComponent<T>(id);
		}

		template<class T>
		bool hasComponent()
		{
			return ECS->hasComponent<T>(id);
		}

		template<class T>
		void RemoveComponent()
		{
			ECS->RemoveComponent(id);
		}

		Transform& getTransform()
		{
			return ECS->getComponent<Transform>(id);
		}

		EntityID getId() const
		{
			return id;
		}

		void destroy()
		{
			ECS->destroyEntity(id);
		}

	private:
		EntityID id;
		EntityComponentSystem* ECS;
	};

}