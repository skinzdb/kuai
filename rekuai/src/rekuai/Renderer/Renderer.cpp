#include "Renderer.h"
#include "rekuai/Renderer/Buffer.h"
#include "rekuai/Renderer/RendererAPI.h"
#include <memory>

namespace kuai {

	std::vector<RenderCmd> Renderer::render_queue = std::vector<RenderCmd>();
	std::unique_ptr<RendererAPI> Renderer::api = RendererAPI::create();

	void Renderer::init()
	{
	    api->init();
	}

	void Renderer::cleanup()
	{

	}

	void Renderer::set_camera(const Camera& camera)
	{
	}

	void Renderer::submit(const RenderCmd &cmd)
    {
	    render_queue.push_back(cmd);
	}

	void Renderer::update() {
	    std::vector<RenderCmd> opaque, transparent;
        for (auto& cmd : render_queue)
        {
            if (cmd.pass == RenderPass::Opaque)
            {
                opaque.push_back(cmd);
            }
            else if (cmd.pass == RenderPass::Transparent)
            {
                transparent.push_back(cmd);
            }
        }

	    std::sort(opaque.begin(), opaque.end(), [](const RenderCmd& x, const RenderCmd& y)
        {
			return x.key.value < y.key.value;
		});

		// Transparent: depth-first (descending); stable-sort by state to reduce flicker
        std::stable_sort(transparent.begin(), transparent.end(), [](const RenderCmd& x, const RenderCmd& y)
        {
            // Here we rely on the depth bucket in the low 8 bits; you can store real depth separately
            uint8_t da = uint8_t(x.key.value & 0xFFu);
            uint8_t db = uint8_t(y.key.value & 0xFFu);
            if (da != db) return da > db; // back-to-front
            return x.key.value < y.key.value; // tie-break by state
        });

       	struct BoundState
        {
            uint32_t program_id = 0;
            uint32_t vao_id = 0;
            uint32_t material_id = 0;
            std::shared_ptr<Shader> shader = nullptr;
            bool blend = false;
        } state;

        // Opaque render pass
        for (const RenderCmd& cmd : opaque)
        {
            if (state.program_id != cmd.program_id)
            {
                state.shader = Shader::get(cmd.program_id);
                state.shader->bind();
                state.program_id = cmd.program_id;
            }

            if (cmd.instanced)
            {

            }
            else
            {
                state.shader->set_uniform("model_matrix", cmd.model);

                // api->draw_indexed(const std::shared_ptr<VertexArray> &vertex_array, 0)
            }
        }
	}
}
