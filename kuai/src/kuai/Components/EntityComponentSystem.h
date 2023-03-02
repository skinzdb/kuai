#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

#include "kuai/Events/Event.h"
#include "kuai/Events/KeyEvent.h"
#include "Components.h"

namespace kuai {

	class EventBus
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		template<typename EventType>
		void notify(EventType& event)
		{
			auto pos = subscribers.find(typeid(EventType).name());

			KU_CORE_ASSERT(pos != subscribers.end(), "No callback set for this event type");

			auto callbacks = pos->second;
			for (auto& c : callbacks)
				c(event);
		}

		template<typename T, typename EventType>
		void subscribe(T& instance, EventCallbackFn fn)
		{
			auto callbacks = subscribers.at(typeid(EventType).name()); // get callbacks or insert and initialise new list if entry does not exist
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

		EntityID createEntity()
		{
			return entityManager->createEntity();
		}

		void destroyEntity(EntityID entity)
		{
			entityManager->destroyEntity(entity);
			componentManager->onEntityDestroyed(entity);
			systemManager->onEntityDestroyed(entity);
		}

		// *** Component Management *******************************************

		template<typename T>
		void registerComponent()
		{
			componentManager->registerComponent<T>();
			KU_CORE_INFO("Registered new component: {0}", typeid(T).name());
		}

		template<typename T, typename ...Args>
		void addComponent(EntityID entity, Args&& ...args)
		{
			componentManager->addComponent<T>(entity, std::forward<Args>(args)...);

			auto componentMask = entityManager->getComponentMask(entity);
			componentMask |= BIT(componentManager->getComponentType<T>());

			entityManager->setComponentMask(entity, componentMask);
			systemManager->onEntityComponentMaskChanged(entity, componentMask);
		}

		template<typename T>
		void removeComponent(EntityID entity)
		{
			componentManager->RemoveComponent(entity);

			auto componentMask = entityManager->getComponentMask(entity);
			componentMask &= BIT(componentManager->getComponentType<T>()) ^ std::numeric_limits<ComponentMask>::max();

			entityManager->setComponentMask(entity, componentMask);
			systemManager->onEntityComponentMaskChanged(entity, componentMask);
		}

		template<typename T>
		T& getComponent(EntityID entity)
		{
			return componentManager->getComponent<T>(entity);
		}

		template<typename T>
		bool hasComponent(EntityID entity)
		{
			return componentManager->hasComponent<T>(entity);
		}

		template<typename T>
		ComponentType getComponentType()
		{
			return componentManager->getComponentType<T>();
		}

		template<typename T>
		void applyToComponents(std::function<void(T component)> fn)
		{
			componentManager->applyToComponents(fn);
		}

		// *** System Management **********************************************

		template<typename T>
		std::shared_ptr<T> registerSystem(Scene* scene)
		{
			KU_CORE_INFO("Registered new system: {0}", typeid(T).name());

			return systemManager->RegisterSystem<T>(scene);
		}

		template<typename T>
		void setSystemMask(ComponentMask mask)
		{
			systemManager->setComponentMask<T>(mask);
		}

		// *** Event Management (of systems) **********************************

		template<typename EventType>
		void notifySystems(EventType& event);

		template<typename T, typename EventType>
		void subscribeSystem(T& instance, EventType& event);

	private:
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<SystemManager> systemManager;

		// *** Event bus - for communication between systems ***
		std::unique_ptr<EventBus> eventBus;
	};
}
