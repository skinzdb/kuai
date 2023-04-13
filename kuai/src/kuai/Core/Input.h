#pragma once

#include "KeyCodes.h"
#include "MouseBtnCodes.h"

namespace kuai {
	/** \class Input
	*	\brief Used to poll mouse and keyboard input, as well as getting input values from the mouse.
	*/
	class Input
	{
	public:
		/**
		* Returns true if key with keycode is being pressed
		*/
		static bool isKeyPressed(KeyCode keycode);

		/**
		* Returns true if mouse button is being pressed
		*/
		static bool isMouseBtnPressed(MouseBtnCode button);

		static std::pair<float, float> getMousePos();
		static float getMouseX();
		static float getMouseY();
	};
}