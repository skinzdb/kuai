#include "kpch.h"

#include "WinWindow.h"

#include "kuai/Events/AppEvent.h"
#include "kuai/Events/MouseEvent.h"
#include "kuai/Events/KeyEvent.h"

namespace kuai {
	static bool glfwInitialised = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		KU_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::create(const WindowProps& props)
	{
		return new WinWindow(props);
	}

	WinWindow::WinWindow(const WindowProps& props)
	{
		init(props);
	}

	WinWindow::~WinWindow()
	{
		cleanup();
	}

	void WinWindow::init(const WindowProps& props)
	{
		KU_PROFILE_FUNCTION();

		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		KU_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if (!glfwInitialised)
		{
			int success = glfwInit();

			KU_CORE_ASSERT(success, "Failed to initialise GLFW");
			
			glfwInitialised = true;
		}

		window = glfwCreateWindow((int)props.width, (int)props.height, props.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // Initialise Glad
		KU_CORE_ASSERT(status, "Failed to initialise Glad");

		glfwSetWindowUserPointer(window, &data); // We can store anything in this user pointer, set it to reference of WindowData struct
		setVSync(true);

		// ************************************************************
		// Set GLFW callbacks		
		// ************************************************************

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height); 
			data.eventCallback(&event); // Dispatch event
		});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventCallback(&event);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressEvent event(key, 0);
					data.eventCallback(&event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent event(key);
					data.eventCallback(&event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressEvent event(key, 1);
					data.eventCallback(&event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseBtnPressEvent event(button);
					data.eventCallback(&event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseBtnReleaseEvent event(button);
					data.eventCallback(&event);
					break;
				}
			}
		});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoff, double yoff)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrollEvent event((float)xoff, (float)yoff);
			data.eventCallback(&event);
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMoveEvent event((float)xpos, (float)ypos);
			data.eventCallback(&event);
		});
	}

	void WinWindow::cleanup()
	{
		glfwDestroyWindow(window);
	}

	void WinWindow::update()
	{
		KU_PROFILE_FUNCTION();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WinWindow::setVSync(bool enabled)
	{
		glfwSwapInterval(enabled);
		data.vSync = enabled;
	}

	bool WinWindow::isVSync() const
	{
		return data.vSync;
	}

}