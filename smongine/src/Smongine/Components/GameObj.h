#pragma once

#include "smpch.h"

namespace Smong {
	class GameObj 
	{
	public:
		GameObj() = default;
		~GameObj() = default;

		// Transform transform; // Every GameObj needs a transform

		void Update();
	};
}