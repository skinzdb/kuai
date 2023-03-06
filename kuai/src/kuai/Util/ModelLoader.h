#pragma once

#include "kuai/Renderer/Mesh.h"

namespace kuai {

	class ModelLoader
	{
	public:
		static std::shared_ptr<Mesh> loadObj(const std::string& filename);
	};
}



