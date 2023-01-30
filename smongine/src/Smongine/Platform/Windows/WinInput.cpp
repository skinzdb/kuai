#include "smpch.h"
#include "Smongine/Core/Input.h"

#include "Smongine/Core/App.h"
#include <GLFW/glfw3.h>

namespace Smong {

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseBtnPressed(MouseBtnCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, keycode);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePos()
	{
		auto window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return std::pair<float, float>((float)xpos, (float)ypos);
	}

	float Input::GetMouseX()
	{
		return Input::GetMousePos().first;
	}

	float Input::GetMouseY()
	{
		return Input::GetMousePos().second;
	}
}


