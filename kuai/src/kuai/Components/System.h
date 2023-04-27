#pragma once

namespace kuai {
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

		virtual void update(float dt) = 0;

		virtual void insertEntity(EntityID entity);

		virtual void removeEntity(EntityID entity);

		std::vector<std::shared_ptr<Entity>>& getEntities();

	protected:
		std::vector<std::shared_ptr<Entity>> entities;

		Scene* scene;

		bool acceptsSubset = false;	// Add entities that have components which are a subset of the system's component mask

	private:

		bool hasEntity(EntityID entity);

		friend class SystemManager;
	};
}