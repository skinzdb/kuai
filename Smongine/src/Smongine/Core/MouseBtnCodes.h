#pragma once

#include "smpch.h"

namespace Smong
{
	using MouseBtnCode = uint16_t;

	enum : MouseBtnCode
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2,
		ButtonLast = Button7
	};
}