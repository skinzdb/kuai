#include "kpch.h"
#include "App.h"

#include "rekuai/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace kuai {

	App* App::instance = nullptr;

	App::App()
	{
		KU_CORE_ASSERT(!instance, "Application already exists");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->set_event_callback(std::bind(&App::on_event, this, std::placeholders::_1));
		running = true;

		ecs = std::make_shared<EntityComponentSystem>();
        ecs->register_component<Transform>();
        ecs->register_component<MeshRenderer>();

        auto render_sys = ecs->register_system<MeshRenderer, Transform>();
        render_sys->each([](float dt, EntityId id, MeshRenderer& mesh_renderer, Transform& transform) {
            Renderer::submit(nullptr, mesh_renderer.mesh, transform.get_model_matrix());
        });

		Renderer::init();
		Renderer::set_viewport(0, 0, window->get_width(), window->get_height());
		//AudioManager::init();
	}

	App::~App()
	{
	    Renderer::stop();
	    ecs.reset();
	    Renderer::cleanup();
		//AudioManager::cleanup();
	}

	std::shared_ptr<Entity> App::create_entity()
	{
	    return std::make_shared<Entity>(ecs);
	}

	void App::run()
	{
		while (running)
		{
			float elapsedTime = timer.get_elapsed(); // Time since last frame
			//KU_CORE_INFO("FPS: {0}", 1.0f / elapsedTime);

			ecs->update(elapsedTime);

			if (!minimised)
			{
				update(elapsedTime);
			}

			Renderer::update();
			window->update();
		}
	}

	void App::on_event(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&App::on_window_close, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&App::on_window_resize, this, std::placeholders::_1));

		input(e);
	}

	bool App::on_window_close(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}

	bool App::on_window_resize(WindowResizeEvent& e)
	{
		if (e.get_width() == 0 || e.get_height() == 0)
		{
			minimised = true;
			return false;
		}

		minimised = false;
		//Renderer::setViewport(0, 0, e.get_width(), e.get_height());

		return false;
	}

}
