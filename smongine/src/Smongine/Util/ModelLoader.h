#pragma once

#include "Smongine/Renderer/Mesh.h"

namespace Smong {

	class ModelLoader
	{
	public:
		std::shared_ptr<Mesh> LoadObj(std::string filename);
	};
}



