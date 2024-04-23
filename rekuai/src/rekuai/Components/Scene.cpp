#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "rekuai/Renderer/Renderer.h"

namespace kuai {
	Scene::Scene() : ecs(new ECS())
	{
		ecs->register_component<Transform>();
		ecs->register_component<MeshRenderer>();
		ecs->register_component<Camera>();

		auto render_sys = ecs->register_system<MeshRenderer, Transform>();

		render_sys->each([](float dt, EntityId entity, MeshRenderer& mesh_renderer, Transform& transform) {

		});

		render_sys->on_insert([](EntityId entity, MeshRenderer& mesh_renderer, Transform& transform) {
			Renderer::add_object(mesh_renderer, transform);
		});

		render_sys->on_remove([](EntityId entity, MeshRenderer& mesh_renderer, Transform& transform) {
			Renderer::remove_object(mesh_renderer, transform);
		});
	}

	Scene::~Scene()
	{
		delete ecs;
	}

	void Scene::update(float dt)
	{
		Renderer::begin_pass();


		Renderer::end_pass();


		Renderer::begin_pass();


		Renderer::end_pass();
	}

	Entity& Scene::create_entity()
	{
		EntityId entity = ecs->create_entity();
		return entities.emplace_back(entity, this);
	}
}