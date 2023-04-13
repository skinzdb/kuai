#pragma once

#include "Core.h"

#include "LayerStack.h"
#include "kuai/Events/Event.h"
#include "kuai/Events/AppEvent.h"
#include "Window.h"
#include "Timer.h"

namespace kuai {
	/** \class App
	*   \brief This class runs your game. It handles windowing, events and layer updates.
	*	It uses layers to execute game logic.
	*	@see Layer
	*/
	class App {
	public:
		App();
		virtual ~App();

		/**
		* Starts the application mainloop which in turn updates all layers.
		*/
		void run();

		/**
		* Called every time an event occurs
		*/
		void onEvent(Event* e);

		/**
		* Pushes a layer on to the layer stack.
		*/
		void pushLayer(Layer* layer);
		/**
		* Pushes an overlay on to the layer stack. Overlays will always get updated last.
		*/
		void pushOverlay(Layer* layer);

		/**
		* Returns the app instance.
		*/
		inline static App& get() { return *instance; }
		/**
		* Returns the window instance.
		*/
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

