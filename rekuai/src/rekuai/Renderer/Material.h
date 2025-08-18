#pragma once

#include "Shader.h"
#include "Texture.h"

namespace kuai {
	class Material
	{
	public:
		Material(const Shader& shader) : shader(shader)
		{
		}

	protected:
		Shader shader;

		friend class Renderer;
	};

	class BasicMaterial : public Material
	{
	public:
		BasicMaterial(const Shader& shader, Texture* diffuse, Texture* normal, const glm::vec2& tiling = { 1.0f, 1.0f })
			: Material(shader), diffuse(diffuse), normal(normal), tiling(tiling), shininess(1.0f)
		{
		}

		Texture* diffuse;
		Texture* normal;

		glm::vec2 tiling;
		float shininess;
	};

}
