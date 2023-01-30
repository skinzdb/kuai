#include "smpch.h"
#include "System.h"

#include "Scene.h"
#include "Entity.h"

namespace Smong {

	void System::InsertEntity(EntityID entity)
	{
		entities.push_back(scene->GetEntityById(entity));
	}

	void System::RemoveEntity(EntityID entity)
	{
		entities.erase(std::remove(entities.begin(), entities.end(), scene->GetEntityById(entity)), entities.end());
	}

	std::vector<std::shared_ptr<Entity>>& System::GetEntities()
	{
		return entities;
	}

}