#pragma once

#include "EntityComponentSystem.h"
#include <memory>

namespace kuai {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void update(float dt);

		Entity& create_entity();

	private:
		std::unique_ptr<EntityComponentSystem> ecs;
		std::vector<Entity> entities;

		friend class Entity;
	};
}
