#pragma once

#include "smpch.h"

#include "Smongine/Core.h"
#include "Smongine/Events/Event.h"

namespace Smong {

	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Smongine",
			unsigned int width = 640,
			unsigned int height = 480)
		{
			this->title = title;
			this->width = width;
			this->height = height;
		}
	};

	// Interface for representing a desktop window
	class SMONGINE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void Update() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}