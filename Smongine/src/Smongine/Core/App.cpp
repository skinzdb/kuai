#include "smpch.h"
#include "App.h"
#include "Log.h"

#include "Smongine/Renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Smong {

	App* App::instance = nullptr;

	App::App() 
	{
		SM_CORE_ASSERT(!instance, "Application already exists");
		instance = this;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(std::bind(&App::OnEvent, this, std::placeholders::_1));
		running = true;

		Renderer::Init();
	}

	App::~App() 
	{

	}

	void App::Run() 
	{
		while (running) 
		{
			float elapsedTime = timer.GetElapsed(); // Time since last frame
						
			if (!minimised)
			{
				for (Layer* layer : layerStack)
					layer->Update(elapsedTime);
			}

			window->Update();
		}

		Renderer::Cleanup();
	}

	void App::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&App::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&App::OnWindowResize, this, std::placeholders::_1));

		for (auto it = layerStack.end(); it != layerStack.begin(); ) // Start from end and go backwards through stack
		{
			(*--it)->OnEvent(e);
			if (e.handled) // As soon as event handled, break
				break;
		}
	}

	void App::PushLayer(Layer* layer)
	{
		layerStack.PushLayer(layer);
	}

	void App::PushOverlay(Layer* layer)
	{
		layerStack.PushOverlay(layer);
	}

	bool App::OnWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}

	bool App::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			minimised = true;
			return false;
		}

		minimised = false;
		Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

}