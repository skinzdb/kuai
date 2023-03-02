#pragma once

#include "KeyCodes.h"
#include "MouseBtnCodes.h"

namespace kuai {
	class Input
	{
	public:
		static bool isKeyPressed(KeyCode keycode);

		static bool isMouseBtnPressed(MouseBtnCode button);
		static std::pair<float, float> getMousePos();
		static float getMouseX();
		static float getMouseY();
	};
}