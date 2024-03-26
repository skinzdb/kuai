#include "kpch.h"
#include "App.h"
#include "Log.h"

#include "rekuai/Renderer/Renderer.h"
#include "rekuai/Components/Components.h"


#include <glad/glad.h>
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

		Renderer::init();
		ecs = new ECS();
		ecs->register_component<Transform>();
		//AudioManager::init();
	}

	App::~App() 
	{
		delete ecs;
	}

	void App::run() 
	{
		while (running) 
		{
			float elapsedTime = timer.get_elapsed(); // Time since last frame
			//KU_CORE_INFO("FPS: {0}", 1.0f / elapsedTime);
						
			if (!minimised)
			{
				update(elapsedTime);
			}

			window->update();
		}

		//AudioManager::cleanup();
		Renderer::cleanup();
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