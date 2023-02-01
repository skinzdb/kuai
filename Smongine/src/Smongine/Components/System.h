#pragma once

#include "Smongine/Events/Event.h"

namespace Smong {
	class Scene; // Forward declarations
	class Entity;

	using EntityID = uint32_t;
	/**
	* Defines the game logic for a set of entities
	*/
	class System
	{
	public:
		System(Scene* scene) : scene(scene) {}

		virtual void Update(float dt) = 0;

		virtual void InsertEntity(EntityID entity);

		virtual void RemoveEntity(EntityID entity);

		std::vector<std::shared_ptr<Entity>>& GetEntities();

	protected:
		std::vector<std::shared_ptr<Entity>> entities;

		Scene* scene;

		bool acceptsSubset = false;	// Add entities that have components which are a subset of the system's component mask

		friend class SystemManager;
	};
}