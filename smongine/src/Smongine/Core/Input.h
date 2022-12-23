#pragma once

#include "Smongine/Core/Core.h"

namespace Smong {
	class Input
	{
	public:
		inline static bool IsKeyPressed(int keycode);

		inline static bool IsMouseBtnPressed(int button);
		inline static std::pair<float, float> GetMousePos();
		inline static float GetMouseX();
		inline static float GetMouseY();
	};
}