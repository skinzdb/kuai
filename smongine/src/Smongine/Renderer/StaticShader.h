#pragma once

#include "Shader.h"

namespace Smong {
	class PhongShader;

	class StaticShader
	{
	public:
		static void Init();
		static void Cleanup();

		static PhongShader* Phong;
	};

	class PhongShader : public Shader
	{
	public:
		PhongShader();

		void SetUniforms(glm::vec3& col, float shininess, float reflectivity);
	};
}