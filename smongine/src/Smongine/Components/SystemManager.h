#pragma once

#include "EntityManager.h"
#include "System.h"

namespace Smong {
	class System;
	/**
	* Manages creation and deletion of all systems
	* Updates each system when an entity's component mask changes
	*/
	class SystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem();

		template<typename T>
		void SetComponentMask(ComponentMask componentMask);

		void OnEntityDestroyed(EntityID entity);
		
		void OnEntityComponentMaskChanged(EntityID entity, ComponentMask entityComponentMask);

	private:
		// Maps each system type name to its system mask (what components does it update)
		std::unordered_map<const char*, ComponentMask> systemMasks;
		// Maps system type names to systems
		std::unordered_map<const char*, std::shared_ptr<System>> systems;
	};

	template<typename T>
	inline std::shared_ptr<T> SystemManager::RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		SM_CORE_ASSERT(systems.find(typeName) == systems.end(), "Registering a system more than once");

		auto system = std::make_shared<T>();
		systems.insert({ typeName, system });
		return system;
	}

	template<typename T>
	inline void SystemManager::SetComponentMask(ComponentMask componentMask)
	{
		const char* typeName = typeid(T).name();

		SM_CORE_ASSERT(systems.find(typeName) != systems.end(), "System not registered");

		systemMasks.insert({ typeName, componentMask });
	}
}