#pragma once

#include "smpch.h"

#include "EntityManager.h"

namespace Smong {

	using ComponentType = uint8_t;

	class IComponentContainer
	{
	public:
		virtual ~IComponentContainer() {}

		virtual void OnEntityDestroyed(EntityID entity) = 0;
	};

	template<typename T>
	class ComponentContainer : public IComponentContainer
	{
	public:
		ComponentContainer() : currentComponents(0) {}

		template <typename ...Args>
		void Insert(EntityID entity, Args&&... args)
		{
			SM_CORE_ASSERT(entityToIndex.find(entity) == entityToIndex.end(), "Added duplicate component to entity");

			entityToIndex[entity] = currentComponents;
			indexToEntity[currentComponents] = entity;
			components.emplace_back(args...);
			currentComponents++;
		}

		void Remove(EntityID entity)
		{
			SM_CORE_ASSERT(entityToIndex.find(entity) != entityToIndex.end(), "Removing component that does not exist");

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

		T& Get(EntityID entity)
		{
			SM_CORE_ASSERT(entityToIndex.find(entity) != entityToIndex.end(), "Retrieving component that does not exist");

			return components[entityToIndex[entity]]; // Return reference to entity's component
		}

		virtual void OnEntityDestroyed(EntityID entity) override
		{
			// If entity owns this component type
			if (entityToIndex.find(entity) != entityToIndex.end())
				Remove(entity);
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
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			SM_CORE_ASSERT(componentTypes.find(typeName) == componentTypes.end(), "Registering a component type more than once")

			componentTypes.insert({ typeName, nextComponentType++ });	// Increment for next component
			componentContainers.insert({ typeName, std::make_shared<ComponentContainer<T>>() });
		}

		template<typename T, typename... Args>
		void AddComponent(EntityID entity, Args&&... args)
		{
			GetComponentContainer<T>()->Insert(entity, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent(EntityID entity)
		{
			GetComponentContainer<T>()->Remove(entity);
		}
		
		template<typename T>
		T& GetComponent(EntityID entity)
		{
			return GetComponentContainer<T>()->Get(entity);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return componentTypes[typeid(T).name()];
		}

		template<typename T>
		void ApplyToComponents(std::function<void(T component)> fn)
		{
			auto container = GetComponentContainer<T>()->begin();
			for (auto const& i : container)
			{
				fn(i);
			}
		}

		void OnEntityDestroyed(EntityID entity)
		{
			for (auto const& pair : componentContainers)
			{
				auto const& component = pair.second;
				component->OnEntityDestroyed(entity);
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
		std::shared_ptr<ComponentContainer<T>> GetComponentContainer()
		{
			const char* typeName = typeid(T).name();

			SM_CORE_ASSERT(componentTypes.find(typeName) != componentTypes.end(), "Component not registered");

			return std::static_pointer_cast<ComponentContainer<T>>(componentContainers[typeName]);
		}
	};
}