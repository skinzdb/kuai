#pragma once

#include "ComponentManager.h"

namespace kuai {

	class ISystem
	{
	public:
		virtual ~ISystem() = default;

		virtual void update(float dt) = 0;
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
		using UpdateFn = std::function<void(float, EntityId, Cs&...)>;
		using InOutFn = std::function<void(EntityId, Cs&...)>;

	public:
		void each(const UpdateFn& action) { update_fn = action; }
		void on_insert(const InOutFn& action) { insert_fn = action; }
		void on_remove(const InOutFn& action) { remove_fn = action; }

	private:
		void update(float dt)
		{
			for (auto entity : entities)
			{
				update_fn(dt, entity, component_manager->get_component<Cs>(entity)...);
			}
		}

		void insert(EntityId entity)
		{
			entities.push_back(entity);
			insert_fn(entity, component_manager->get_component<Cs>(entity)...);
		}

		void remove(EntityId entity)
		{
			size_t orig = entities.size();
			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
			if (entities.size() != orig)
			{
				remove_fn(entity, component_manager->get_component<Cs>(entity)...);
			}
		}

		UpdateFn update_fn;
		InOutFn insert_fn;
		InOutFn remove_fn;
	};
}
