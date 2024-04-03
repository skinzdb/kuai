#pragma once

#include "ECS.h"

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
		ECS* ecs;
		std::vector<Entity> entities;

		friend class Entity;
	};
}