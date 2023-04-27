#include "kpch.h"
#include "System.h"

#include "Scene.h"

namespace kuai {

	void System::insertEntity(EntityID entity)
	{
		entities.push_back(scene->getEntityById(entity));
	}

	void System::removeEntity(EntityID entity)
	{
		entities.erase(std::remove(entities.begin(), entities.end(), scene->getEntityById(entity)), entities.end());
	}

	std::vector<std::shared_ptr<Entity>>& System::getEntities()
	{
		return entities;
	}

	bool System::hasEntity(EntityID entity)
	{
		return std::find(entities.begin(), entities.end(), scene->getEntityById(entity)) != entities.end();
	}
}