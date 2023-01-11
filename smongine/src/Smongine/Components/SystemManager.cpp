#include "smpch.h"
#include "SystemManager.h"

namespace Smong {
	void SystemManager::OnEntityDestroyed(EntityID entity)
	{
		for (auto const& pair : systems)
		{
			auto const& system = pair.second;
			system->RemoveEntity(entity);
		}
	}

	void SystemManager::OnEntityComponentMaskChanged(EntityID entity, ComponentMask entityComponentMask)
	{
		// Update each system
		for (auto const& pair : systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemComponentMask = systemMasks[type];

			// If entity's component mask matches this system, add it to the system's list
			if (entityComponentMask == systemComponentMask || (system->acceptsSubset && (entityComponentMask & systemComponentMask).any()))
				system->InsertEntity(entity);
			else
				system->RemoveEntity(entity);
		}
	}
}


