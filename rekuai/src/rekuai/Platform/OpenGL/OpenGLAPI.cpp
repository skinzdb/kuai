#include "OpenGLAPI.h"

#include "glad/glad.h"
#include "rekuai/Renderer/Buffer.h"
#include "rekuai/Core/Util.h"

namespace kuai {
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         KU_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       KU_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          KU_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: KU_CORE_TRACE(message); return;
		}
		
		KU_CORE_ASSERT(false, "Unknown severity level");
	}

    void OpenGLAPI::init() {
       	glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // #ifdef KU_DEBUG
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(OpenGLMessageCallback, nullptr);
            
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	    // #endif

		std::string vert_src = read_file("shader.vert");
		std::string frag_src = read_file("shader.frag");
		shader = std::make_shared<OpenGLShader>(vert_src, frag_src);
        shader->bind();
    }

    void OpenGLAPI::stop() 
    {

    }

    OpenGLAPI::~OpenGLAPI() 
    {

    }

    // Shader::set_uniform("proj_matrix", )

		// for (auto& shader : r_data.shaders)
		// {
		// 	shader.bind();

		// 	size_t cmd_count = r_data.shader_map[shader.id].mesh_to_cmd.size();

		// 	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (void*)0, cmd_count, sizeof(IndirectCommand));
		// }

    void OpenGLAPI::draw_indexed(std::shared_ptr<VertexArray> vertex_array, uint32_t index_count)
    {
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
