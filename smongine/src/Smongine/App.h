#pragma once

#include "Core.h"

#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/AppEvent.h"
#include "Window.h"

namespace Smong {
	class SMONGINE_API App {
	public:
		App();
		virtual ~App();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> window;
		bool running;
		LayerStack layerStack;
	};

	App* CreateApp();
}

