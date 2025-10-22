#pragma once

#include "ComponentManager.h"

namespace kuai {

	class ISystem
	{
	public:
		virtual ~ISystem() = default;

		virtual void insert(EntityId entity) = 0;
		virtual void remove(EntityId entity) = 0;

		std::vector<EntityId>::iterator begin() { return entities.begin(); }
		std::vector<EntityId>::iterator end() { return entities.end(); }

	protected:
		std::shared_ptr<ComponentManager> component_manager;

		std::vector<EntityId> entities;

		friend class SystemManager;
	};

	template <typename ...Cs>
	class System : public ISystem
	{
	public:
		template<typename Fn>
		void each(Fn&& func)
		{
			for (auto entity : entities)
			{
				func(component_manager->get_component<Cs>(entity)...);
			}
		}

	private:
		virtual void insert(EntityId entity) override
		{
			entities.push_back(entity);
		}

		virtual void remove(EntityId entity) override
		{
			size_t orig = entities.size();
			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
		}
	};
}
