#pragma once

#include "Smongine/Core/Layer.h"

namespace Smong {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void Update();
		void OnEvent(Event& event);
	private:
		float time;
	};
}


