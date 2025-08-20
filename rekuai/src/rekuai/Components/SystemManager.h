#pragma once

#include "rekuai/Core/Core.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "System.h"

namespace kuai {
	/**
	* Manages creation and deletion of all systems
	* Updates each system when an entity's component mask changes
	*/
	class SystemManager
	{
	public:
		SystemManager() = default;
		~SystemManager()
		{
			for (const auto& pair : systems)
			{
				delete pair.second;
			}
		}

		template<typename... Cs>
		System<Cs...>* register_system(ComponentManager* component_manager)
		{
			ComponentMask system_mask = 0;

			for (const auto mask : { component_manager->get_component_type<Cs>()... })
			{
				system_mask |= mask;
			}

			KU_CORE_ASSERT(systems.find(system_mask) == systems.end(), "Registering a system more than once");

			auto system = new System<Cs...>();
			system->component_manager = component_manager;
			systems.insert({ system_mask, system });
			return system;
		}

		void on_entity_destroyed(EntityId entity)
		{
			for (auto const& pair : systems)
			{
				auto const& system = pair.second;
				system->remove(entity);
			}
		}

		void on_component_mask_changed(EntityId entity, ComponentMask entity_component_mask)
		{
			// Update each system
			for (const auto& [system_mask, system] : systems)
			{
				// If entity's component mask matches this system, add it to the system's list
				if ((entity_component_mask & system_mask) == system_mask)
				{
					system->insert(entity);
				}
				else
				{
					system->remove(entity);
				}
			}
		}

		void update(float dt)
		{
			for (const auto& pair : systems)
			{
				pair.second->update(dt);
			}
		}

	private:
		// Maps system masks to systems
		std::unordered_map<ComponentMask, SystemBase*> systems;
	};
}
