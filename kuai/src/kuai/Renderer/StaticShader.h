#pragma once

#include "Shader.h"

namespace kuai {
	// Forward declaration
	class Light;

	const uint32_t MAX_LIGHTS = 10;

	class IShader : public Shader
	{
	public:
		IShader(const std::string& vertSrc, const std::string& fragSrc);

		virtual void update() = 0;

		virtual void updateLight() = 0;
		virtual void updateTransform();
		virtual void updateCamera();

	protected:
		struct ShaderData
		{
			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 modelMatrix;

			glm::vec3 viewPos;

			Light* light;
		};

		std::shared_ptr<ShaderData> shaderData;

	public:
		std::shared_ptr<ShaderData> getData() { return shaderData; }
	};

	class DefaultShader : public IShader
	{
	public:
		DefaultShader();

		virtual void update() {}
		virtual void updateLight();
		virtual void updateCamera() override;
	};

	class SkyboxShader : public IShader
	{
	public:
		SkyboxShader();
		
		virtual void update() {}
		virtual void updateLight() {}
		virtual void updateTransform() override {}
	};

	class StaticShader
	{
	public:
		static void init();
		static void cleanup();

		static IShader* basic;
		static IShader* skybox;

		static uint32_t getUboId(const std::string& name);

	private:
		static std::unordered_map<std::string, uint32_t> ubos;
	};
}


