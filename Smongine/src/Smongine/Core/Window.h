#pragma once

#include "smpch.h"

#include "Smongine/Core/Core.h"
#include "Smongine/Events/Event.h"

namespace Smong {

	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "Demo",
			uint32_t width = 640,
			uint32_t height = 480)
		{
			this->title = title;
			this->width = width;
			this->height = height;
		}
	};

	// Interface for representing a desktop window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event*)>;

		virtual ~Window() {}

		virtual void Update() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}