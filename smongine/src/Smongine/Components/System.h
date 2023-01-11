#pragma once

#include "Smongine/Events/Event.h"
#include "Scene.h"

namespace Smong {
	class Scene; // Forward Declare
	class Entity;

	/**
	* Defines the game logic for a set of entities
	*/
	class System
	{
	public:
		virtual void Init(std::shared_ptr<Scene> scene) = 0;
		virtual void Update(float dt) = 0;

		void InsertEntity(EntityID entity);
		void RemoveEntity(EntityID entity);

	protected:
		std::vector<Entity*> entities;

		bool acceptsSubset = false;	// Add entities that have components which are a subset of the system's component mask

		std::shared_ptr<Scene> scene;

		friend class SystemManager;
	};

	class CameraSystem : public System
	{
		virtual void Init(std::shared_ptr<Scene> scene)
		{
			this->scene = scene;
		}

		virtual void Update(float dt);
	};
}