#pragma once

#include "smpch.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace Smong {
	class EntityComponentSystem
	{
	public:
		void Init()
		{
			entityManager = std::make_unique<EntityManager>();
			componentManager = std::make_unique<ComponentManager>();
			systemManager = std::make_unique<SystemManager>();
		}

		EntityID CreateEntity()
		{
			return entityManager->CreateEntity();
		}

		void DestroyEntity(EntityID entity)
		{
			entityManager->DestroyEntity(entity);
			componentManager->OnEntityDestroyed(entity);
			systemManager->OnEntityDestroyed(entity);
		}

		template<typename T, typename... Args>
		void AddComponent(EntityID entity, Args&&... args)
		{
			componentManager->AddComponent<T>(entity, std::forward<Args>(args)...);

			auto componentMask = entityManager->GetComponentMask(entity);
			componentMask.set(componentManager->GetComponentType<T>(), true);
			entityManager->SetComponentMask(entity, componentMask);

			systemManager->OnEntityComponentMaskChanged(entity, componentMask);
		}

		template<typename T>
		void RemoveComponent(EntityID entity)
		{
			componentManager->RemoveComponent(entity);

			auto componentMask = entityManager->GetComponentMask(entity);
			componentMask.set(componentManager->GetComponentType<T>(), false);
			entityManager->SetComponentMask(entity, componentMask);

			systemManager->OnEntityComponentMaskChanged(entity, componentMask);
		}

		template<typename T>
		T& GetComponent(EntityID entity)
		{
			return componentManager->GetComponent<T>(entity);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return componentManager->GetComponentType<T>();
		}

		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			return systemManager->RegisterSystem<T>();
		}

		template<typename T>
		void SetSystemMask(ComponentMask mask)
		{
			systemManager->SetComponentMask<T>(mask);
		}

		template<typename T, typename... Args>
		void SetSystemMask(ComponentMask mask, Args&&... args)
		{
			systemManager->SetComponentMask<T>(mask);

			SetSystemMask<T>(args ...);
		}
	private:
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<SystemManager> systemManager;
	};
}