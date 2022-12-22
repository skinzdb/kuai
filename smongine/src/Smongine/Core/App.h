#pragma once

#include "Core.h"

#include "LayerStack.h"
#include "Smongine/Events/Event.h"
#include "Smongine/Events/AppEvent.h"
#include "Window.h"
#include "Timer.h"

namespace Smong {
	class SMONGINE_API App {
	public:
		App();
		virtual ~App();

		void Run();
		void Update();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static App& Get() { return *instance; }
		inline Window& GetWindow() { return *window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		 
		std::unique_ptr<Window> window;
		Timer timer;
		bool running;
		LayerStack layerStack;

		static App* instance;
	};

	App* CreateApp();
}

