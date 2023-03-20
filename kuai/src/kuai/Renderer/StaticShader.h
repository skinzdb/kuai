#pragma once

#include "Shader.h"

namespace kuai {

	class DefaultShader : public Shader
	{
	public:
		DefaultShader();

		virtual void update();
	};

	class SkyboxShader : public Shader
	{
	public:
		SkyboxShader();
		
		virtual void update();
	};

	class StaticShader
	{
	public:
		static void init();
		static void cleanup();

		static Shader* basic;
		static Shader* skybox;
	};
}

