#pragma once

#include "kpch.h"

#include "rekuai/Core/Core.h"
#include "rekuai/Events/Event.h"

namespace kuai {

	/**
	* Properties of a window, i.e., title, width and height.
	*/
	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "Demo",
			uint32_t width = 1024,
			uint32_t height = 768)
		{
			this->title = title;
			this->width = width;
			this->height = height;
		}
	};

	/**
	* Interface for representing a desktop window
	*/
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void update() = 0;

		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;

		virtual void set_event_callback(const EventCallbackFn& callback) = 0;
		virtual void set_vsync(bool enabled) = 0;
		virtual bool is_vsync() const = 0;

		virtual void* get_native_window() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}