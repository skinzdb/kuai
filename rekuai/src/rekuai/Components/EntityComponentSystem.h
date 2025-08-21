#pragma once

#include <memory>
#include <list>

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

#include "rekuai/Events/Event.h"

namespace kuai {

	typedef void (*Handler)(EventType&);
	using HandlerList = std::list<Handler>;

	class EventBus
	{
	public:
		template<typename EventType>
		void notify(EventType& event)
		{
			auto handlers = subscribers[typeid(EventType).name()];

			if (!handlers)
				return;

			for (auto& handler : *handlers)
			{
				if (handler != nullptr)
				{
					handler(event);
				}
			}
		}

		template<typename EventType>
		void subscribe(void (*handler)(EventType&))
		{
			auto handlers = subscribers[typeid(EventType).name()];

			if (!handlers) // First time init
			{
				handlers = std::make_shared<HandlerList>();
				subscribers[typeid(EventType).name()] = handlers;
			}

			handlers->push_back(handler);
		}
	private:
		std::unordered_map<const char*, std::shared_ptr<HandlerList>> subscribers;
	};

	class EntityComponentSystem
	{
	public:
		EntityComponentSystem() :
		    entity_manager(std::make_unique<EntityManager>()),
		    component_manager(std::make_shared<ComponentManager>()),
			sys_manager(std::make_unique<SystemManager>()),
			event_bus(std::make_unique<EventBus>())
		{
		}

		// *** Entity Management **********************************************

		EntityId create_entity()
		{
			return entity_manager->create_entity();
		}

		void destroy_entity(EntityId id)
		{
			sys_manager->on_entity_destroyed(id);
			entity_manager->destroy_entity(id);
			component_manager->on_entity_destroyed(id);
		}

		// *** Component Management *******************************************

		template<typename T>
		void register_component()
		{
			component_manager->register_component<T>();
		}

		template<typename T, typename ...Args>
		void add_component(EntityId id, Args&& ...args)
		{
			component_manager->add_component<T>(id, std::forward<Args>(args)...);

			auto componentMask = entity_manager->get_component_mask(id);
			componentMask |= BIT(component_manager->get_component_type<T>());

			entity_manager->set_component_mask(id, componentMask);
			sys_manager->on_component_mask_changed(id, componentMask);
		}

		template<typename T>
		void remove_component(EntityId id)
		{
			component_manager->remove_component<T>(id);

			auto componentMask = entity_manager->get_component_mask(id);
			componentMask &= BIT(component_manager->get_component_type<T>()) ^ std::numeric_limits<ComponentMask>::max();

			entity_manager->set_component_mask(id, componentMask);
			sys_manager->on_component_mask_changed(id, componentMask);
		}

		template<typename T>
		T& get_component(EntityId id)
		{
			return component_manager->get_component<T>(id);
		}

		template<typename T>
		bool has_component(EntityId id)
		{
			return component_manager->has_component<T>(id);
		}

		template<typename T>
		ComponentType get_component_type()
		{
			return component_manager->get_component_type<T>();
		}

		// *** System Management **********************************************

		template<typename ...Cs>
		std::shared_ptr<System<Cs...>> register_system()
		{
			return sys_manager->register_system<Cs...>(component_manager);
		}

		// *** Event Management (of systems) **********************************

		template<typename EventType>
		void notify_systems(EventType& event)
		{
			event_bus->notify(event);
		}

		template<typename T, typename EventType>
		void subscribe_system(void (handler)(EventType&))
		{
			event_bus->subscribe<T>(handler);
		}

	private:
		std::unique_ptr<EntityManager> entity_manager;
		std::shared_ptr<ComponentManager> component_manager;
		std::unique_ptr<SystemManager> sys_manager;

		// *** Event bus - for communication between systems ***
		std::unique_ptr<EventBus> event_bus;
	};
}
