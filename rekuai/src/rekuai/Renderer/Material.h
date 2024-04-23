#pragma once

#include "Shader.h"
#include "Texture.h"

namespace kuai {
	class Material
	{
	public:

	private:
		Shader* shader;

		friend class Renderer;
	};

	class BasicMaterial : public Material
	{
	public:
		BasicMaterial(Texture* diffuse, Texture* normal, const glm::vec2& tiling = { 1.0f, 1.0f })
			: diffuse(diffuse), normal(normal), tiling(tiling), shininess(1.0f)
		{
		}
	
		Texture* diffuse;
		Texture* normal;

		glm::vec2 tiling;
		float shininess;
	};

}