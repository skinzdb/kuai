#include "smpch.h"

#include "WinWindow.h"
#include "Smongine/Log.h"

namespace Smong {
	static bool glfwInitialised = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WinWindow(props);
	}

	WinWindow::WinWindow(const WindowProps& props)
	{
		Init(props);
	}

	WinWindow::~WinWindow()
	{
		Shutdown();
	}

	void WinWindow::Init(const WindowProps& props)
	{
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		SM_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if (!glfwInitialised)
		{
			int success = glfwInit();
			SM_CORE_ASSERT(success, "Could not initialise GLFW");
			
			glfwInitialised = true;
		}

		window = glfwCreateWindow((int)props.width, (int)props.height, props.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &data);
		SetVSync(true);
	}

	void WinWindow::Shutdown()
	{
		glfwDestroyWindow(window);
	}

	void WinWindow::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WinWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled);
		data.vSync = enabled;
	}

	bool WinWindow::IsVSync() const
	{
		return data.vSync;
	}

}