#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Smong {
	class SMONGINE_API App {
	public:
		App();
		virtual ~App();

		void Run();

	private:
		std::unique_ptr<Window> window;
		bool running;
	};

	App* CreateApp();
}

