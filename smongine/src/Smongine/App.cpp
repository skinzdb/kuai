#include <GLFW/glfw3.h>

#include "smpch.h"
#include "App.h"
#include "Log.h"

namespace Smong {

	App::App() {
		window = std::unique_ptr<Window>(Window::Create());
		running = true;
		window->SetEventCallback(std::bind(&App::OnEvent, this, std::placeholders::_1));
	}

	App::~App() {

	}

	void App::Run() {
		while (running) {
			// glClearColor(1, 0, 0, 1);
			// glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : layerStack)
				layer->Update();

			window->Update();
		}
	}

	void App::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&App::OnWindowClose, this, std::placeholders::_1));

		SM_CORE_TRACE("{0}", e);

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

}