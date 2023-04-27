#include "kpch.h"
#include "App.h"
#include "Log.h"

#include "kuai/Renderer/Renderer.h"
#include "kuai/Sound/AudioManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace kuai {

	App* App::instance = nullptr;

	App::App() 
	{
		KU_CORE_ASSERT(!instance, "Application already exists");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(std::bind(&App::onEvent, this, std::placeholders::_1));
		running = true;

		Renderer::init();
		AudioManager::init();
	}

	App::~App() 
	{

	}

	void App::run() 
	{
		while (running) 
		{
			float elapsedTime = timer.getElapsed(); // Time since last frame
			KU_CORE_INFO("FPS: {0}", 1.0f / elapsedTime);
						
			if (!minimised)
			{
				{
					KU_PROFILE_SCOPE("LayerStack update");

					for (Layer* layer : layerStack)
						layer->update(elapsedTime);
				}
			}

			window->update();
		}

		AudioManager::cleanup();
		Renderer::cleanup();
	}

	void App::onEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&App::onWindowClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&App::onWindowResize, this, std::placeholders::_1));

		for (auto it = layerStack.end(); it != layerStack.begin(); ) // Start from end and go backwards through stack
		{
			(*--it)->onEvent(e);
			if (e->handled) // As soon as event handled, break
				break;
		}
	}

	void App::pushLayer(Layer* layer)
	{
		layerStack.pushLayer(layer);
	}

	void App::pushOverlay(Layer* layer)
	{
		layerStack.pushOverlay(layer);
	}

	bool App::onWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}

	bool App::onWindowResize(WindowResizeEvent& e)
	{
		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			minimised = true;
			return false;
		}

		minimised = false;
		Renderer::setViewport(0, 0, e.getWidth(), e.getHeight());

		return false;
	}

}