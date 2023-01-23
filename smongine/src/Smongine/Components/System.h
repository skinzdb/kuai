#pragma once

#include "Smongine/Events/Event.h"

namespace Smong {
	/**
	* Defines the game logic for a set of entities
	*/
	class System
	{
	public:
		virtual void Update(float dt) = 0;

		void InsertEntity(EntityID entity)
		{
			entities.push_back(entity);
			//entities.push_back(scene->GetEntityById(entity));
		}

		void RemoveEntity(EntityID entity)
		{
			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
			//entities.erase(std::remove(entities.begin(), entities.end(), scene->GetEntityById(entity)), entities.end());
		}

		std::vector<EntityID>& GetEntities()
		{
			return entities;
		}

	protected:
		std::vector<EntityID> entities;

		bool acceptsSubset = false;	// Add entities that have components which are a subset of the system's component mask

		//std::shared_ptr<Scene> scene;

		friend class SystemManager;
	};
}