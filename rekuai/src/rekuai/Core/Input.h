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
		static bool is_key_pressed(KeyCode keycode);

		/**
		* Returns true if mouse button is being pressed
		*/
		static bool is_mouse_pressed(MouseBtnCode button);

		static std::pair<float, float> get_mouse_pos();
		static float get_mouse_x();
		static float get_mouse_y();
	};
}