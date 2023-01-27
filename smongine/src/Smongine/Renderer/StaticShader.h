#pragma once

#include "Shader.h"

#define NUM_LIGHTS 10

namespace Smong {
	class PhongShader;
	class SimpleShader;

	class StaticShader
	{
	public:
		static void Init();
		static void Cleanup();

		static PhongShader* Phong;
		static SimpleShader* Simple;
	};

	class PhongShader : public Shader
	{
	public:
		PhongShader();

		void SetUniforms(float shininess);
	};

	class SimpleShader : public Shader
	{
	public:
		SimpleShader();

		void SetUniforms(glm::vec3& col);
	};
}