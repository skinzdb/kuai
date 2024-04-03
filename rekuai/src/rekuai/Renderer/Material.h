#pragma once

#include "Shader.h"

namespace kuai {
	class Material
	{
	public:
		Material() = default;


	private:
		//std::vector<Texture> textures;
		

		Shader* shader;

		friend class Renderer;
	};
}