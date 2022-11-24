#pragma once

namespace Smong {
	class SMONGINE_API Renderer
	{
	public:
		static void Init();
		static void Cleanup();

		static void Render();
	};
}

