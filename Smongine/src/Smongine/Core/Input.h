#pragma once

#include "KeyCodes.h"
#include "MouseBtnCodes.h"

namespace Smong {
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseBtnPressed(MouseBtnCode button);
		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};
}