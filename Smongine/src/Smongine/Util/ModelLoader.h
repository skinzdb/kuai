#pragma once

#include "Smongine/Renderer/Mesh.h"

namespace Smong {

	class ModelLoader
	{
	public:
		static Mesh* LoadObj(std::string filename);
	};
}



