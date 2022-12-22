#pragma once

#include "smpch.h"

namespace Smong {

	const uint32_t MAX_ENTITIES = 4096;	// Includes reserved null entity
	const uint32_t MAX_COMPONENTS = 32;

	const uint32_t NULL_ENTITY = 0;

	using EntityID = uint32_t;
	using ComponentMask = std::bitset<MAX_COMPONENTS>;

	/**
	* Manages creation and deletion of all entities (game objects)
	* Entities are stored as a unique integer ID
	* Signatures are bitsets that show what components an entity has
	*/
	class EntityManager
	{
	public:
		EntityManager()
		{
			entityNo = 1;
			for (EntityID entity = 1; entity < MAX_ENTITIES; entity++)
				availableEntities.push_back(entity);
		}

		EntityID CreateEntity()
		{
			SM_CORE_ASSERT(entityNo < MAX_ENTITIES, "Exceeded maximum number of entities");

			EntityID entity = availableEntities.back();
			availableEntities.pop_back();

			componentMasks[entity] = 0;

			entityNo++;
		}

		void DestroyEntity(EntityID entity)
		{
			componentMasks[entity].reset();

			availableEntities.push_back(entity);
			entityNo--;
		}

		ComponentMask GetComponentMask(EntityID entity)
		{
			return componentMasks[entity];
		}

		void SetComponentMask(EntityID entity, ComponentMask componentMask)
		{
			componentMasks[entity] = componentMask;
		}

	private:
		// List of unused entity IDs
		std::vector<EntityID> availableEntities;
		// Number of entities currently in use
		size_t entityNo;

		// Components associated with each entity
		std::array<ComponentMask, MAX_ENTITIES> componentMasks;
	};
}