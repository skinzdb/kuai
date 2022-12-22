#pragma once

#include "EntityComponentSystem.h"
#include "Entity.h"

namespace Smong {

	class SMONGINE_API Scene
	{
	public:
		Scene();
		~Scene();

		Entity* CreateEntity();
		Entity* GetEntityById(EntityID entity);
		void DestroyEntity(EntityID entity);

		

	private:
		void Update(float dt);

		std::shared_ptr<EntityComponentSystem> ECS;
		std::unordered_map<EntityID, Entity*> entityMap;
	};
}


