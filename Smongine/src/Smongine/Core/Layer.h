#pragma once

#include "Smongine/Core/Core.h"
#include "Smongine/Events/Event.h"

namespace Smong {
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void Update(float dt) {}
		virtual void OnEvent(Event* event) {}

		inline const std::string& GetName() const { return name;  }
	protected:
		std::string name;
	};
}


