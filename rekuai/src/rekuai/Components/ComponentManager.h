#pragma once

#include "kpch.h"

// @cond
namespace kuai {
	using ComponentType = u8;

	class IComponentContainer
	{
	public:
		virtual ~IComponentContainer() = default;

		virtual void on_entity_destroyed(EntityId entity) = 0;
	};

	template<typename T>
	class ComponentContainer : public IComponentContainer
	{
	public:
		template <typename ...Args>
		void insert(EntityId entity, Args&&... args)
		{
			KU_CORE_ASSERT(entity_to_idx.find(entity) == entity_to_idx.end(), "Added duplicate component to entity");

			entity_to_idx[entity] = size;
			idx_to_entity[size] = entity;

			components[size] = make_box<T>(args...);

			size++;
		}

		void remove(EntityId entity)
		{
			KU_CORE_ASSERT(entity_to_idx.find(entity) != entity_to_idx.end(), "Removing component that does not exist");

			// Update mappings s.t. entity of last component in array points to removed index and vice-versa
			size_t remove_idx = entity_to_idx[entity];
			size_t last_idx = size;

			EntityId last_entity = idx_to_entity[last_idx];
			entity_to_idx[last_entity] = remove_idx;
			idx_to_entity[remove_idx] = last_entity;

			// Move last element of array to removed index so components are mapped properly
			components[remove_idx] = std::move(components[last_idx]);

			// Erase mappings of removed component and index of last component in the array
			// The components array will remain tightly packed
			entity_to_idx.erase(entity);
			idx_to_entity.erase(last_idx);

			last_idx--;
		}

		T& get(EntityId entity)
		{
			KU_CORE_ASSERT(entity_to_idx.find(entity) != entity_to_idx.end(), "Retrieving component that does not exist");

			return *(components[entity_to_idx[entity]].get()); // Return reference to entity's component
		}

		bool has(EntityId entity)
		{
			return entity_to_idx.find(entity) != entity_to_idx.end();
		}

		virtual void on_entity_destroyed(EntityId entity) override
		{
			// If entity owns this component type
			if (entity_to_idx.find(entity) != entity_to_idx.end())
			{
				remove(entity);
			}
		}

		typename std::vector<T>::iterator begin() { return components.begin(); }
		typename std::vector<T>::iterator end() { return components.end(); }

	private:
		Box<T> components[MAX_ENTITIES];

		// Mappings from entities to their respective index in components array and vice-versa
		std::unordered_map<EntityId, size_t> entity_to_idx;
		std::unordered_map<size_t, EntityId> idx_to_entity;

		size_t size = 0;
	};

	class ComponentManager
	{
	public:
		template<typename T>
		void register_component()
		{
			const char* ty_name = typeid(T).name();

			KU_CORE_ASSERT(component_types.find(ty_name) == component_types.end(), "Registering a component type more than once")

			component_types.emplace(ty_name, next_component_type++);	// Increment for next component
			component_containers.emplace(ty_name, make_rc<ComponentContainer<T>>());
		}

		template<typename T, typename... Args>
		void add_component(EntityId entity, Args&&... args)
		{
			get_component_container<T>()->insert(entity, std::forward<Args>(args)...);
		}

		template<typename T>
		void remove_component(EntityId entity)
		{
			get_component_container<T>()->remove(entity);
		}
		
		template<typename T>
		T& get_component(EntityId entity)
		{
			return get_component_container<T>()->get(entity);
		}

		template<typename T>
		ComponentType get_component_type() const
		{
			return component_types.at(typeid(T).name());
		}

		template<typename T>
		bool has_component(EntityId entity) const
		{
			return get_component_container<T>()->has(entity);
		}

		void on_entity_destroyed(EntityId entity)
		{
			for (auto const& pair : component_containers)
			{
				auto const& component = pair.second;
				component->on_entity_destroyed(entity);
			}
		}
		
	private:
		// Map of component names to their types (uint_8)
		std::unordered_map<const char*, ComponentType> component_types;
		// Map of component names to their containers
		std::unordered_map<const char*, Rc<IComponentContainer>> component_containers;

		// Component type to be assigned to next registered component
		ComponentType next_component_type = 0;

		template<typename T>
		Rc<ComponentContainer<T>> get_component_container()
		{
			const char* ty_name = typeid(T).name();

			KU_CORE_ASSERT(component_types.find(ty_name) != component_types.end(), "Component not registered");

			return std::static_pointer_cast<ComponentContainer<T>>(component_containers[ty_name]);
		}
	};
}
// @endcond
