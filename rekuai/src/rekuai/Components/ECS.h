#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace kuai {

	class ECS 
	{
	public:
		ECS()
		{
			entity_manager = new EntityManager();
			component_manager = new ComponentManager();
			system_manager = new SystemManager();
		}

		~ECS()
		{
			delete entity_manager;
			delete component_manager;
			delete system_manager;
		}

		void update(float dt)
		{
			system_manager->update(dt);
		}

		// *** Entity Management **********************************************

		EntityId create_entity()
		{
			return entity_manager->create_entity();
		}

		void destroy_entity(EntityId entity)
		{
			system_manager->on_entity_destroyed(entity);
			entity_manager->destroy_entity(entity);
			component_manager->on_entity_destroyed(entity);
		}

		// *** Component Management *******************************************

		template<typename T>
		void register_component()
		{
			component_manager->register_component<T>();
			KU_CORE_INFO("Registered new component: {0}", typeid(T).name());
		}

		template<typename T, typename ...Args>
		void add_component(EntityId entity, Args&& ...args)
		{
			component_manager->add_component<T>(entity, std::forward<Args>(args)...);

			auto component_mask = entity_manager->get_component_mask(entity);
			component_mask |= BIT(component_manager->get_component_type<T>());

			entity_manager->set_component_mask(entity, component_mask);
			system_manager->on_component_mask_changed(entity, component_mask);
		}

		template<typename T>
		void remove_component(EntityId entity)
		{
			component_manager->remove_component<T>(entity);

			auto component_mask = entity_manager->get_component_mask(entity);
			component_mask &= BIT(component_manager->get_component_type<T>()) ^ std::numeric_limits<ComponentMask>::max();

			entity_manager->set_component_mask(entity, component_mask);
			system_manager->on_component_mask_changed(entity, component_mask);
		}

		template<typename T>
		T& get_component(EntityId entity)
		{
			return component_manager->get_component<T>(entity);
		}

		template<typename T>
		bool has_component(EntityId entity)
		{
			return component_manager->has_component<T>(entity);
		}

		template<typename T>
		ComponentType get_component_type()
		{
			return component_manager->get_component_type<T>();
		}

		// *** System Management **********************************************

		template<typename ...Cs>
		Rc<System<Cs...>> register_system()
		{
			return system_manager->register_system<Cs...>(component_manager);
		}

		template<typename T>
		void set_system_mask(ComponentMask mask)
		{
			system_manager->set_component_mask<T>(mask);
		}

	private:
		EntityManager* entity_manager;
		ComponentManager* component_manager;
		SystemManager* system_manager;
	};
}