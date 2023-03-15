#pragma once

#include "kpch.h"

#include "kuai/Core/Core.h"
#include "kuai/Events/Event.h"

namespace kuai {

	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "Demo",
			uint32_t width = 1280,
			uint32_t height = 720)
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

		virtual void update() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}