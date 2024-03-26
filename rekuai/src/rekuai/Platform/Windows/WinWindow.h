#pragma once

#include "rekuai/Core/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// @cond
namespace kuai {
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowProps& props);
		virtual ~WinWindow();

		void update() override;

		inline unsigned int get_width() const override { return data.width; }
		inline unsigned int get_height() const override { return data.height; }

		inline void set_event_callback(const EventCallbackFn& callback) override
		{
			data.event_callback = callback;
		}
		virtual void set_vsync(bool enabled) override;
		virtual bool is_vsync() const override;

		virtual void* get_native_window() const { return window; }
	private:
		virtual void init(const WindowProps& props);
		virtual void cleanup();

		GLFWwindow* window;

		struct WindowData
		{
			std::string title;
			uint32_t width, height;
			bool vsync;

			EventCallbackFn event_callback;
		};

		WindowData data;
	};
}
// @endcond