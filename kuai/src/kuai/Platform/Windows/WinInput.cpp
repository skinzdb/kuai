#include "kpch.h"
#include "kuai/Core/Input.h"

#include "kuai/Core/App.h"
#include <GLFW/glfw3.h>

namespace kuai {

	bool Input::isKeyPressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(App::get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isMouseBtnPressed(MouseBtnCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(App::get().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, keycode);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::getMousePos()
	{
		auto window = static_cast<GLFWwindow*>(App::get().getWindow().getNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return std::pair<float, float>((float)xpos, (float)ypos);
	}

	float Input::getMouseX()
	{
		return Input::getMousePos().first;
	}

	float Input::getMouseY()
	{
		return Input::getMousePos().second;
	}
}


