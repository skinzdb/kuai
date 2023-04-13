#pragma once

#include "kpch.h"

#include "EntityManager.h"

// @cond
namespace kuai {
	using ComponentType = uint8_t;

	class IComponentContainer
	{
	public:
		virtual ~IComponentContainer() {}

		virtual void onEntityDestroyed(EntityID entity) = 0;
	};

	template<typename T>
	class ComponentContainer : public IComponentContainer
	{
	public:
		ComponentContainer() : currentComponents(0) {}

		template <typename ...Args>
		void insert(EntityID entity, Args&&... args)
		{
			KU_CORE_ASSERT(entityToIndex.find(entity) == entityToIndex.end(), "Added duplicate component to entity");

			entityToIndex[entity] = currentComponents;
			indexToEntity[currentComponents] = entity;
			components.emplace_back(args...);
			currentComponents++;
		}

		void remove(EntityID entity)
		{
			KU_CORE_ASSERT(entityToIndex.find(entity) != entityToIndex.end(), "Removing component that does not exist");

			// Update mappings s.t. entity of last component in array points to removed index and vice-versa
			size_t removeIndex = entityToIndex[entity];

			EntityID lastElementEntity = indexToEntity[currentComponents - 1];
			entityToIndex[lastElementEntity] = removeIndex;
			indexToEntity[removeIndex] = lastElementEntity;

			// Erase mappings of removed component and index of last component in the array
			entityToIndex.erase(entity);
			indexToEntity.erase(--currentComponents);
			// The components array will remain tightly packed
		}

		T& get(EntityID entity)
		{
			KU_CORE_ASSERT(entityToIndex.find(entity) != entityToIndex.end(), "Retrieving component that does not exist");

			return components[entityToIndex[entity]]; // Return reference to entity's component
		}

		bool has(EntityID entity)
		{
			return entityToIndex.find(entity) != entityToIndex.end();
		}

		virtual void onEntityDestroyed(EntityID entity) override
		{
			// If entity owns this component type
			if (entityToIndex.find(entity) != entityToIndex.end())
				remove(entity);
		}

		typename std::vector<T>::iterator begin() { return components.begin(); }
		typename std::vector<T>::iterator end() { return components.end(); }

	private:
		std::vector<T> components;

		// Mappings from entities to their respective index in components array and vice-versa
		std::unordered_map<EntityID, size_t> entityToIndex;
		std::unordered_map<size_t, EntityID> indexToEntity;

		size_t currentComponents;
	};

	class ComponentManager
	{
	public:
		template<typename T>
		void registerComponent()
		{
			const char* typeName = typeid(T).name();

			KU_CORE_ASSERT(componentTypes.find(typeName) == componentTypes.end(), "Registering a component type more than once")

			componentTypes.insert({ typeName, nextComponentType++ });	// Increment for next component
			componentContainers.insert({ typeName, std::make_shared<ComponentContainer<T>>() });
		}

		template<typename T, typename... Args>
		void addComponent(EntityID entity, Args&&... args)
		{
			getComponentContainer<T>()->insert(entity, std::forward<Args>(args)...);
		}

		template<typename T>
		void removeComponent(EntityID entity)
		{
			getComponentContainer<T>()->remove(entity);
		}
		
		template<typename T>
		T& getComponent(EntityID entity)
		{
			return getComponentContainer<T>()->get(entity);
		}

		template<typename T>
		bool hasComponent(EntityID entity)
		{
			return getComponentContainer<T>()->has(entity);
		}

		template<typename T>
		ComponentType getComponentType()
		{
			return componentTypes[typeid(T).name()];
		}

		template<typename T>
		void applyToComponents(std::function<void(T component)> fn)
		{
			auto container = getComponentContainer<T>()->begin();
			for (auto const& i : container)
			{
				fn(i);
			}
		}

		void onEntityDestroyed(EntityID entity)
		{
			for (auto const& pair : componentContainers)
			{
				auto const& component = pair.second;
				component->onEntityDestroyed(entity);
			}
		}
		
	private:
		// Map of component names to their types (uint_8)
		std::unordered_map<const char*, ComponentType> componentTypes;
		// Map of component names to their containers
		std::unordered_map<const char*, std::shared_ptr<IComponentContainer>> componentContainers;

		// Component type to be assigned to next registered component
		ComponentType nextComponentType = 0;

		template<typename T>
		std::shared_ptr<ComponentContainer<T>> getComponentContainer()
		{
			const char* typeName = typeid(T).name();

			KU_CORE_ASSERT(componentTypes.find(typeName) != componentTypes.end(), "Component not registered");

			return std::static_pointer_cast<ComponentContainer<T>>(componentContainers[typeName]);
		}
	};
}
// @endcond
