#pragma once

#include "Shader.h"
#include "glm/glm.hpp"

namespace kuai {
	// Forward declaration
	class Light;

	const uint32_t MAX_LIGHTS = 10;

	class DefaultShader : public Shader
	{
	public:
		DefaultShader();
	};

	class SkyboxShader : public Shader
	{
	public:
		SkyboxShader();
	};

	class DepthShader : public Shader
	{
	public:
		DepthShader();
	};

	class StaticShader
	{
	public:
		static void init();
		static void cleanup();

		static Shader* basic;
		static Shader* skybox;
		static Shader* depth;
	};
}


