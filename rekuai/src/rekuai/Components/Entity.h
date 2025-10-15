#pragma once

#include "rekuai/Components/EntityComponentSystem.h"

// The entity class is a wrapper for an EntityId
namespace kuai {
	/** \class Entity
	*	\brief Base class for all game objects.
	*/
	class Entity
	{
	public:
		Entity(std::shared_ptr<EntityComponentSystem> ecs)
		    : id(ecs->create_entity()), ecs(ecs) {}

		/**
		* Add a component T that can be constructed with ...args to this entity.
		*/
		template<class T, typename ...Args>
		T& add_component(Args&& ...args)
		{
			ecs->add_component<T>(id, std::forward<Args>(args)...);
			return ecs->get_component<T>(id);
		}

		/**
		* Return component T if it is attached to this entity.
		*/
		template<class T>
		T& get_component() const
		{
			return ecs->get_component<T>(id);
		}

		/**
		* Check if component T is attached to this entity.
		*/
		template<class T>
		bool has_component() const
		{
			return ecs->has_component<T>(id);
		}

		/**
		* Removes component T from this entity.
		*/
		template<class T>
		void remove_component()
		{
			ecs->remove_component<T>(id);
		}

		EntityId get_id() const
		{
			return id;
		}

		operator uint32_t() const { return (uint32_t)id; }

		operator bool() const { return id != null_entity; }

		bool operator==(const Entity& other) const
		{
			return id == other.id;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		EntityId id;
		std::shared_ptr<EntityComponentSystem> ecs;
	};

}
