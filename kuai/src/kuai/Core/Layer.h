#pragma once

#include "kuai/Core/Core.h"
#include "kuai/Events/Event.h"

namespace kuai {
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void update(float dt) {}
		virtual void onEvent(Event* event) {}

		inline const std::string& getName() const { return name;  }
	protected:
		std::string name;
	};
}


