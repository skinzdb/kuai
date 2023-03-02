#pragma once

#include "Shader.h"

#define NUM_LIGHTS 10

namespace kuai {
	class PhongShader;
	class SimpleShader;

	class StaticShader
	{
	public:
		static void init();
		static void cleanup();

		static PhongShader* Phong;
		static SimpleShader* Simple;
	};

	class PhongShader : public Shader
	{
	public:
		PhongShader();

		void setUniforms(float shininess);
	};

	class SimpleShader : public Shader
	{
	public:
		SimpleShader();

		void setUniforms(const glm::vec3& col) const;
	};
}