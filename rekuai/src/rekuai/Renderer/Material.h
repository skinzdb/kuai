#pragma once

#include "Shader.h"

namespace kuai {
	class Material
	{
	public:
		Material(std::shared_ptr<Shader> shader) : shader(shader)
		{
		}

		std::shared_ptr<Shader> get_shader() { return shader; }

		uint32_t get_id() { return 0; }

	private:
		std::shared_ptr<Shader> shader;
	};

	// class BasicMaterial : public Material
	// {
	// public:
	// 	BasicMaterial(const Shader& shader, Texture* diffuse, Texture* normal, const glm::vec2& tiling = { 1.0f, 1.0f })
	// 		: Material(shader), diffuse(diffuse), normal(normal), tiling(tiling), shininess(1.0f)
	// 	{
	// 	}

	// 	Texture* diffuse;
	// 	Texture* normal;

	// 	glm::vec2 tiling;
	// 	float shininess;
	// };

}
