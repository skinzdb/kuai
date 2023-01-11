#include "smpch.h"
#include "System.h"

namespace Smong {
	void System::InsertEntity(EntityID entity)
	{
		//entities.push_back(entity);
		entities.push_back(scene->GetEntityById(entity));
	}

	void System::RemoveEntity(EntityID entity)
	{
		//entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
		entities.erase(std::remove(entities.begin(), entities.end(), scene->GetEntityById(entity)), entities.end());
	}

	void CameraSystem::Update(float dt)
	{
		for (auto& entity : entities)
		{
			SM_CORE_INFO("{0}", static_cast<int>(entity->GetId()));
		}
	}
}