#pragma once

#include "Smongine/Components/Scene.h"

namespace Smong {
	class Renderer
	{
	public:
		static void Render(Scene& scene);

		static void Init();
		static void Cleanup();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearCol(glm::vec4& col);
		static void Clear();
	};
}

