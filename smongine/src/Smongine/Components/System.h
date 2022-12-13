#pragma once

#include "smpch.h"

namespace Smong {
	/**
	* Defines the game logic for a set of entities
	*/
	class System
	{
	public:
		virtual void Init() = 0;
		virtual void Update() = 0;

		void InsertEntity(EntityID entity)
		{
			entities.push_back(entity);
		}

		void RemoveEntity(EntityID entity)
		{
			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
		}

		std::vector<EntityID> entities;
	};
}