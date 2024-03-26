#pragma once

#include "ComponentManager.h"

namespace kuai {

	class SystemBase 
	{
	public:
		virtual ~SystemBase() {}

		std::vector<EntityId>::iterator begin() { return entities.begin(); }
		std::vector<EntityId>::iterator end() { return entities.end(); }

	protected:
		virtual void update(float dt) = 0;

	protected:
		ComponentManager* component_manager;

		std::vector<EntityId> entities;
		ComponentMask mask;

		friend class SystemManager;
	};

	template<class... Cs>
	class System : public SystemBase
	{
		using ActionFn = std::function<void(float, Cs&...)>;

	public:
		System() = default;

		void each(const ActionFn& action)
		{
			this->action = action;
		}

	private:
		void update(float dt)
		{
			for (auto entity : entities)
			{
				action(dt, { component_manager->get_component<Cs...>(entity) });
			}
		}

		ActionFn action;
	};
}