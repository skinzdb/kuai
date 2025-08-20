#include "OpenGLAPI.h"

#include "glad/glad.h"
#include "rekuai/Renderer/Buffer.h"
#include <cstdint>

namespace kuai {
    void OpenGLAPI::init() {
       	glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    OpenGLAPI::~OpenGLAPI() {

    }

    // Shader::set_uniform("proj_matrix", )

		// for (auto& shader : r_data.shaders)
		// {
		// 	shader.bind();

		// 	size_t cmd_count = r_data.shader_map[shader.id].mesh_to_cmd.size();

		// 	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (void*)0, cmd_count, sizeof(IndirectCommand));
		// }

    void OpenGLAPI::draw_indexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count) {
        vertex_array->bind();
        uint32_t count = index_count ? index_count : vertex_array->get_index_count();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLAPI::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        glViewport(x, y, width, height);
    }
}
