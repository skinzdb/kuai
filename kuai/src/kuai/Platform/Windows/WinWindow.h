#pragma once

#include "kuai/Core/Window.h"

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

		inline unsigned int getWidth() const override { return data.width; }
		inline unsigned int getHeight() const override { return data.height; }

		inline void setEventCallback(const EventCallbackFn& callback) override
		{
			data.eventCallback = callback;
		}
		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

		virtual void* getNativeWindow() const { return window; }
	private:
		virtual void init(const WindowProps& props);
		virtual void cleanup();

		GLFWwindow* window;

		struct WindowData
		{
			std::string title;
			uint32_t width, height;
			bool vSync;

			EventCallbackFn eventCallback;
		};

		WindowData data;
	};
}
// @endcond