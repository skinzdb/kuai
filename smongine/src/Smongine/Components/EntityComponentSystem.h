#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

#include "Smongine/Events/Event.h"
#include "Smongine/Events/KeyEvent.h"
#include "Components.h"

namespace Smong {
	class SystemManager;

	class EventBus
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		template<typename EventType>
		void notify(EventType& event)
		{
			auto pos = subscribers.find(typeid(EventType).name());

			SM_CORE_ASSERT(pos != subscribers.end(), "No callback set for this event type");

			auto callbacks = pos->second;
			for (auto& c : callbacks)
				c(event);
		}

		template<typename T, typename EventType>
		void subscribe(T& instance, EventCallbackFn fn)
		{
			auto callbacks = subscribers[typeid(EventType).name()]; // Get callbacks or insert and initialise new list if entry does not exist
			callbacks->push_back(fn);
		}
	private:
		std::unordered_map<const char*, std::vector<EventCallbackFn>> subscribers;
	};

	class EntityComponentSystem
	{
	public:
		void Init();

		// *** Entity Management *********************************

		EntityID CreateEntity();
		void DestroyEntity(EntityID entity);

		// *** Component Management ******************************

		template<typename T>
		void RegisterComponent();

		template<typename T, typename... Args>
		void AddComponent(EntityID entity, Args&&... args);

		template<typename T>
		void RemoveComponent(EntityID entity);

		template<typename T>
		T& GetComponent(EntityID entity);

		template<typename T>
		ComponentType GetComponentType();

		template<typename T>
		void ApplyToComponents(std::function<void(T component)> fn);

		// *** System Management ******************************

		template<typename T>
		std::shared_ptr<T> RegisterSystem();

		template<typename T>
		void SetSystemMask(ComponentMask mask);

		template<typename T, typename... Args>
		void SetSystemMask(ComponentMask mask, Args&&... args);

	private:
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<SystemManager> systemManager;
	public:
		// *** Event bus - for communication between systems ***
		std::unique_ptr<EventBus> eventBus;
	};

	//class IFnHandler
	//{
	//public:
	//	void exec(Event& e)
	//	{
	//		call(e);
	//	}

	//private:
	//	virtual void call(Event& e) = 0;
	//};

	//class FnHandler : IFnHandler
	//{

	//};

	template<typename T>
	inline void EntityComponentSystem::RegisterComponent()
	{
		componentManager->RegisterComponent<T>();
		SM_CORE_INFO("Registered new component: {0}", typeid(T).name());
	}

	template<typename T, typename ...Args>
	inline void EntityComponentSystem::AddComponent(EntityID entity, Args && ...args)
	{
		componentManager->AddComponent<T>(entity, std::forward<Args>(args)...);

		auto componentMask = entityManager->GetComponentMask(entity);
		componentMask.set(componentManager->GetComponentType<T>(), true);
		entityManager->SetComponentMask(entity, componentMask);

		systemManager->OnEntityComponentMaskChanged(entity, componentMask);
	}

	template<typename T>
	inline void EntityComponentSystem::RemoveComponent(EntityID entity)
	{
		componentManager->RemoveComponent(entity);

		auto componentMask = entityManager->GetComponentMask(entity);
		componentMask.set(componentManager->GetComponentType<T>(), false);
		entityManager->SetComponentMask(entity, componentMask);

		systemManager->OnEntityComponentMaskChanged(entity, componentMask);
	}

	template<typename T>
	inline T& EntityComponentSystem::GetComponent(EntityID entity)
	{
		return componentManager->GetComponent<T>(entity);
	}

	template<typename T>
	inline ComponentType EntityComponentSystem::GetComponentType()
	{
		return componentManager->GetComponentType<T>();
	}

	template<typename T>
	inline void EntityComponentSystem::ApplyToComponents(std::function<void(T component)> fn)
	{
		componentManager->ApplyToComponents(fn);
	}

	template<typename T>
	inline std::shared_ptr<T> EntityComponentSystem::RegisterSystem()
	{
		{
			SM_CORE_INFO("Registered new system: {0}", typeid(T).name());

			return systemManager->RegisterSystem<T>();
		}

	}

	template<typename T>
	inline void EntityComponentSystem::SetSystemMask(ComponentMask mask)
	{
		systemManager->SetComponentMask<T>(mask);
	}

	template<typename T, typename ...Args>
	inline void EntityComponentSystem::SetSystemMask(ComponentMask mask, Args && ...args)
	{
		systemManager->SetComponentMask<T>(mask);

		SetSystemMask<T>(args ...);
	}
}