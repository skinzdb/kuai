#pragma once

namespace kuai {

	const u32 MAX_ENTITIES = 10000;
	const u32 MAX_COMPONENTS = 32;

	using EntityId = u32;
	using ComponentMask = u32;

	static const EntityId null_entity = 0;

	/**
	* Manages creation and deletion of all entities (game objects)
	* Entities are stored as a unique integer ID
	* Component masks are bitsets that show what components an entity has
	*/
	class EntityManager
	{
	public:
		EntityManager()
		{
			for (size_t i = 1; i <= MAX_ENTITIES; i++)
			{
				available.push_back(MAX_ENTITIES - i);
			}
		}

		EntityId create_entity()
		{
			KU_CORE_ASSERT(entity_count < MAX_ENTITIES, "Exceeded maximum number of entities");

			EntityId entity = available.back();
			available.pop_back();
			component_masks[entity] = 0;
			entity_count++;

			return entity;
		}

		void destroy_entity(EntityId entity)
		{
			component_masks[entity] = 0;

			available.push_back(entity);
			entity_count--;
		}

		ComponentMask get_component_mask(EntityId entity) const
		{
			return component_masks[entity];
		}

		void set_component_mask(EntityId entity, ComponentMask mask)
		{
			component_masks[entity] = mask;
		}

	private:
		// List of unused entity IDs
		std::vector<EntityId> available;
		// Number of entities currently in use
		u32 entity_count = 0;

		// Components associated with each entity
		ComponentMask component_masks[MAX_ENTITIES];
	};
}
