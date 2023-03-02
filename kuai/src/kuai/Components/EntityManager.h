#pragma once

namespace kuai {

	const uint32_t MAX_ENTITIES = 4096;	// Includes reserved null entity
	const uint32_t MAX_COMPONENTS = 32;

	const uint32_t NULL_ENTITY = 0;

	using EntityID = uint32_t;
	using ComponentMask = uint32_t;

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
			entityNo = 0;
			for (EntityID entity = 1; entity < MAX_ENTITIES; entity++)
				availableEntities.push_back(entity);
		}

		EntityID createEntity()
		{
			KU_CORE_ASSERT(entityNo < MAX_ENTITIES, "Exceeded maximum number of entities");

			EntityID entity = availableEntities.back();
			availableEntities.pop_back();
			componentMasks[entity] = 0;
			entityNo++;

			return entity;
		}

		void destroyEntity(EntityID entity)
		{
			componentMasks[entity] = 0;

			availableEntities.push_back(entity);
			entityNo--;
		}

		ComponentMask getComponentMask(EntityID entity)
		{
			return componentMasks[entity];
		}

		void setComponentMask(EntityID entity, ComponentMask componentMask)
		{
			componentMasks[entity] = componentMask;
		}

	private:
		// List of unused entity IDs
		std::vector<EntityID> availableEntities;
		// Number of entities currently in use
		uint32_t entityNo;

		// Components associated with each entity
		ComponentMask componentMasks[MAX_ENTITIES];
	};
}