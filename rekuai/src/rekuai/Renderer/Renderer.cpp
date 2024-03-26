#include "Renderer.h"

#include "glad/glad.h"

namespace kuai {

	void Renderer::init()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::cleanup()
	{

	}

	void Renderer::set_viewport(u32 x, u32 y, u32 width, u32 height)
	{
		glViewport(x, y, width, height);
	}

	void Renderer::set_clear_col(const glm::vec4& col)
	{
		glClearColor(col.r, col.g, col.b, col.a);
	}

	void Renderer::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}

