#pragma once

#include "glm/glm.hpp"

#include "Framebuffer.h"
#include "Mesh.h"
#include "Material.h"

#include "RendererAPI.h"
#include "rekuai/Components/Components.h"
#include <cstdint>

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

	enum RenderPass : uint8_t 
	{
	    Opaque = 0,
		Transparent = 1,
		Shadow = 2
	};

	struct SortKey 
	{
	    uint64_t value = 0;

        SortKey(RenderPass pass, uint16_t prog, uint16_t material, uint16_t mesh, uint8_t depth_bucket) {
            value = (uint64_t(pass) << 56)
                  | (uint64_t(prog) << 40)
                  | (uint64_t(material) << 24)
                  | (uint64_t(mesh) << 8)
                  | depth_bucket;
        }
	};

	struct RenderCmd 
	{
        SortKey key;
        RenderPass pass;
        uint32_t program_id;
        uint32_t material_id;
        uint32_t mesh_id;
        bool instanced;
        glm::mat4 model;

        RenderCmd(RenderPass pass, uint32_t program_id, uint32_t material_id, uint32_t mesh_id, bool instanced, glm::mat4 model) :
            key(SortKey(pass, uint16_t(program_id), uint16_t(material_id), uint16_t(mesh_id), 0)),
            pass(pass),
            program_id(program_id),
            material_id(material_id),
            mesh_id(mesh_id),
            instanced(instanced),
            model(model) {}
	};

	class Renderer
	{
	public:
		static void init();
		static void cleanup();

		static void set_camera(const Camera& camera);
		static void submit(const RenderCmd& cmd);
		static void update();

	private:
	    static std::vector<RenderCmd> render_queue;

		static std::unique_ptr<RendererAPI> api;
	};
}
