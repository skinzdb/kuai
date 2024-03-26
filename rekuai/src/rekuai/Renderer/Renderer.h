#pragma once

#include "glm/glm.hpp"

namespace kuai {

	class Renderer
	{
	public:
		static void init();
		static void cleanup();	

		static void begin_pass(/*framebuffer to write to, struct for render flags (e.g. clear GL_DEPTH) */);
		static void end_pass();

		//static void submit(Shader* shader);

		static void set_viewport(u32 x, u32 y, u32 width, u32 height);
		static void set_clear_col(const glm::vec4& col);
		static void clear();

	private:

	};
}

