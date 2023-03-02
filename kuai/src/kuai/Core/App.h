#pragma once

#include "Core.h"

#include "LayerStack.h"
#include "kuai/Events/Event.h"
#include "kuai/Events/AppEvent.h"
#include "Window.h"
#include "Timer.h"

namespace kuai {
	class App {
	public:
		App();
		virtual ~App();

		void run();

		void onEvent(Event* e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		inline static App& get() { return *instance; }
		inline Window& getWindow() { return *window; }
	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> window;
		Timer timer;
		bool running = true;
		bool minimised = false;
		LayerStack layerStack;

		static App* instance;
	};
}

