#include "Scene.h"

#include "Entity.h"
#include "Components.h"

namespace kuai {
	Scene::Scene() : ecs(std::make_unique<EntityComponentSystem>())
	{
		ecs->register_component<Transform>();
		ecs->register_component<MeshRenderer>();
		ecs->register_component<Camera>();

		auto render_sys = ecs->register_system<MeshRenderer, Transform>();

		render_sys->each([&](float dt, EntityId entity, MeshRenderer& mesh_renderer, Transform& transform) {
		    auto poo = ecs->get_component<MeshRenderer>(entity);
		});

		render_sys->on_insert([](EntityId entity, MeshRenderer& mesh_renderer, Transform& transform) {

		});

		render_sys->on_remove([](EntityId entity, MeshRenderer& mesh_renderer, Transform& transform) {

		});
	}

	Scene::~Scene()
	{
	}

	void Scene::update(float dt)
	{

	}

	Entity& Scene::create_entity()
	{
		EntityId entity = ecs->create_entity();
		return entities.emplace_back(entity, this);
	}
}
