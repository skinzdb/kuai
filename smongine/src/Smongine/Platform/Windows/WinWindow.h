#pragma once

#include "Smongine/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Smong {
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowProps& props);
		virtual ~WinWindow();

		void Update() override;

		inline unsigned int GetWidth() const override { return data.width; }
		inline unsigned int GetHeight() const override { return data.height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override
		{
			data.eventCallback = callback;
		}
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

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