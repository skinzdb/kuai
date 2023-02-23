#pragma once

#include "Smongine/Renderer/Mesh.h"

namespace Smong {

	class ModelLoader
	{
	public:
		static std::shared_ptr<Mesh> LoadObj(std::string filename);
	};
}



