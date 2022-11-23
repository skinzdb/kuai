#pragma once

#include "Smongine/Core.h"
#include "Smongine/Events/Event.h"

namespace Smong {
	class SMONGINE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void Update() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return name;  }
	protected:
		std::string name;
	};
}


