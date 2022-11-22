#include "smpch.h"
#include "App.h"

#include "Events/KeyEvent.h"
#include "Log.h"

namespace Smong {

	App::App() {
		window = std::unique_ptr<Window>(Window::Create());
		running = true;
	}

	App::~App() {

	}

	void App::Run() {
		while (running) {
			window->Update();
		}
	}

}