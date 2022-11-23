#pragma once

#include "Smongine/Core.h"

namespace Smong {
	class SMONGINE_API Input
	{
	public:
		static bool IsKeyPressed(int keycode) { return instance->IsKeyPressedImpl(keycode);  }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
	private:
		static Input* instance;
	};
}