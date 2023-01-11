#include "smpch.h"
#include "EntityComponentSystem.h"

namespace Smong {

	void EntityComponentSystem::Init()
	{
		entityManager = std::make_unique<EntityManager>();
		componentManager = std::make_unique<ComponentManager>();
		systemManager = std::make_unique<SystemManager>();
		eventBus = std::make_unique<EventBus>();
	}

	EntityID EntityComponentSystem::CreateEntity()
	{
		return entityManager->CreateEntity();
	}

	void EntityComponentSystem::DestroyEntity(EntityID entity)
	{
		entityManager->DestroyEntity(entity);
		componentManager->OnEntityDestroyed(entity);
		systemManager->OnEntityDestroyed(entity);
	}
}