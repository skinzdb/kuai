#pragma once

#include "Core.h"

#include "rekuai/Events/Event.h"
#include "rekuai/Events/AppEvent.h"

#include "rekuai/Components/ECS.h"

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

		virtual void update(float dt) = 0;

		virtual void input(Event& e) {}

		/**
		* Returns the app instance.
		*/
		inline static App& get() { return *instance; }
		/**
		* Returns the window instance.
		*/
		inline Window& get_window() { return *window; }
	private:
		/**
		* Called every time an event occurs
		*/
		void on_event(Event& e);

		bool on_window_close(WindowCloseEvent& e);
		bool on_window_resize(WindowResizeEvent& e);

	protected:
		ECS* ecs;

	private:
		std::unique_ptr<Window> window;
		Timer timer;
		bool running = true;
		bool minimised = false;

		static App* instance;
	};

	// To be defined in client
	App* create_app();
}

