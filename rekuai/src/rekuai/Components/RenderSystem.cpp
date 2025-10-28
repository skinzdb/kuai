#include "RenderSystem.h"

#include "rekuai/Renderer/RendererAPI.h"

namespace kuai
{
    void kuai::RenderSystem::update()
    {
        std::vector<RenderCmd> opaque, transparent;
        for (const auto &cmd : render_cmds)
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

        std::sort(opaque.begin(), opaque.end(), [](const RenderCmd &x, const RenderCmd &y)
                  { return x.key.value < y.key.value; });

        // Transparent: depth-first (descending); stable-sort by state to reduce flicker
        std::stable_sort(transparent.begin(), transparent.end(), [](const RenderCmd &x, const RenderCmd &y)
                         {
                             // Here we rely on the depth bucket in the low 8 bits; you can store real depth separately
                             uint8_t da = uint8_t(x.key.value & 0xFFu);
                             uint8_t db = uint8_t(y.key.value & 0xFFu);
                             if (da != db)
                                 return da > db;               // back-to-front
                             return x.key.value < y.key.value; // tie-break by state
                         });

        struct BoundState
        {
            uint16_t program_id = 0;
            uint16_t material_id = 0;
            uint16_t vao_id = 0;
            std::shared_ptr<Shader> shader = nullptr;
            bool blend = false;
        } state;

        // Opaque render pass
        for (const RenderCmd &cmd : opaque)
        {
            if (state.program_id != cmd.program_id)
            {
                state.shader = Shader::get(cmd.program_id);
                state.shader->bind();
                state.program_id = cmd.program_id;
            }

            if (state.vao_id != cmd.mesh_id)
            {
                state.vao_id = cmd.mesh_id;
            }

            if (cmd.instanced)
            {
            }
            else
            {
                // cmd.mesh_id
                state.shader->set_uniform("model_matrix", cmd.transform.get_model_matrix());
                
                // RendererAPI::get()->draw_indexed(state.shader, {}, 0);
            }
        }
    }

    void RenderSystem::insert(EntityId entity)
    {
        const auto &mesh_renderer = component_manager->get_component<MeshRenderer>(entity);
        const auto &transform = component_manager->get_component<Transform>(entity);

        render_cmds.emplace_back(
            Opaque,
            mesh_renderer.material->get_shader()->get_id(),
            mesh_renderer.material->get_id(),
            mesh_renderer.mesh->get_id(),
            false,
            transform
        );
        System::insert(entity);
    }

    void RenderSystem::remove(EntityId entity)
    {
        // render_cmds.

        System::remove(entity);
    }
}