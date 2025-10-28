#pragma once

#include "glm/glm.hpp"

#include "Mesh.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "rekuai/Components/Components.h"

namespace kuai {

	enum RenderFlag
	{
		NEVER,
		LT,
		EQ,
		LE,
		GT,
		NE,
		GE,
		ALWAYS,
		DEPTH,
		COLOUR,
		STENCIL
	};

	class Renderer
	{
	public:
		static void init();
		static void stop();
		static void cleanup();

		static void set_camera(const Camera& camera);
		static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void submit(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, const glm::mat4& model);
		static void clear();

	private:
		static std::unique_ptr<RendererAPI> api;

		struct SceneData
		{
			glm::mat4 view_matrix;
			glm::mat4 proj_matrix;
		};

		struct ModelViewProj
		{
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};

		static std::unique_ptr<SceneData> scene_data;
	};
}
