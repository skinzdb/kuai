#include "kpch.h"
#include "rekuai/Core/Input.h"

#include "rekuai/Core/App.h"
#include <GLFW/glfw3.h>

namespace kuai {

	bool Input::is_key_pressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(App::get().get_window().get_native_window());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::is_mouse_pressed(MouseBtnCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(App::get().get_window().get_native_window());
		auto state = glfwGetMouseButton(window, keycode);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::get_mouse_pos()
	{
		auto window = static_cast<GLFWwindow*>(App::get().get_window().get_native_window());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return std::make_pair((float)xpos, (float)ypos);
	}

	float Input::get_mouse_x()
	{
		return Input::get_mouse_pos().first;
	}

	float Input::get_mouse_y()
	{
		return Input::get_mouse_pos().second;
	}
}


