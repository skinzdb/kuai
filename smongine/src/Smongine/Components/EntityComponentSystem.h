#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

#include "Smongine/Events/Event.h"
#include "Smongine/Events/KeyEvent.h"
#include "Components.h"

namespace Smong {

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
			auto callbacks = subscribers.at(typeid(EventType).name()); // Get callbacks or insert and initialise new list if entry does not exist
			callbacks->push_back(fn);
		}
	private:
		std::unordered_map<const char*, std::vector<EventCallbackFn>> subscribers;
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

	class EntityComponentSystem
	{
	public:
		EntityComponentSystem()
		{
			entityManager = std::make_unique<EntityManager>();
			componentManager = std::make_unique<ComponentManager>();
			systemManager = std::make_unique<SystemManager>();
			eventBus = std::make_unique<EventBus>();
		}

		// *** Entity Management **********************************************

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

		// *** Component Management *******************************************

		template<typename T>
		void RegisterComponent()
		{
			componentManager->RegisterComponent<T>();
			SM_CORE_INFO("Registered new component: {0}", typeid(T).name());
		}

		template<typename T, typename ...Args>
		void AddComponent(EntityID entity, Args && ...args)
		{
			componentManager->AddComponent<T>(entity, std::forward<Args>(args)...);

			auto componentMask = entityManager->GetComponentMask(entity);
			componentMask |= BIT(componentManager->GetComponentType<T>());

			entityManager->SetComponentMask(entity, componentMask);
			systemManager->OnEntityComponentMaskChanged(entity, componentMask);
		}

		template<typename T>
		void RemoveComponent(EntityID entity)
		{
			componentManager->RemoveComponent(entity);

			auto componentMask = entityManager->GetComponentMask(entity);
			componentMask &= BIT(componentManager->GetComponentType<T>()) ^ std::numeric_limits<ComponentMask>::max();

			entityManager->SetComponentMask(entity, componentMask);
			systemManager->OnEntityComponentMaskChanged(entity, componentMask);
		}

		template<typename T>
		T& GetComponent(EntityID entity)
		{
			return componentManager->GetComponent<T>(entity);
		}

		template<typename T>
		bool HasComponent(EntityID entity)
		{
			return componentManager->HasComponent<T>(entity);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return componentManager->GetComponentType<T>();
		}

		template<typename T>
		void ApplyToComponents(std::function<void(T component)> fn)
		{
			componentManager->ApplyToComponents(fn);
		}

		// *** System Management **********************************************

		template<typename T>
		std::shared_ptr<T> RegisterSystem(Scene* scene)
		{
			SM_CORE_INFO("Registered new system: {0}", typeid(T).name());

			return systemManager->RegisterSystem<T>(scene);
		}

		template<typename T>
		void SetSystemMask(ComponentMask mask)
		{
			systemManager->SetComponentMask<T>(mask);
		}

		// *** Event Management (of systems) **********************************

		template<typename EventType>
		void NotifySystems(EventType& event);

		template<typename T, typename EventType>
		void SubscribeSystem(T& instance, EventType& event)
		{
			eventBus->subscribe(T, event);
		}

	private:
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<SystemManager> systemManager;

		// *** Event bus - for communication between systems ***
		std::unique_ptr<EventBus> eventBus;
	};
}
