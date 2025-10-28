#pragma once

#include "System.h"
#include "Components.h"

namespace kuai
{
	enum RenderPass : uint8_t
	{
	    Opaque = 0,
		Transparent = 1,
		Shadow = 2
	};

	struct SortKey
	{
	    uint64_t value = 0;

        SortKey(RenderPass pass, uint16_t prog, uint16_t material, uint16_t mesh, uint8_t depth_bucket) 
        {
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
        const Transform &transform;

        RenderCmd(RenderPass pass, uint32_t program_id, uint32_t material_id, uint32_t mesh_id, bool instanced, const Transform &transform) :
            key(SortKey(pass, uint16_t(program_id), uint16_t(material_id), uint16_t(mesh_id), 0)),
            pass(pass),
            program_id(program_id),
            material_id(material_id),
            mesh_id(mesh_id),
            instanced(instanced),
            transform(transform) 
        {
        }
	};

    class RenderSystem : public System<MeshRenderer, Transform>
    {
    public:
        void update();

    private:
        virtual void insert(EntityId entity) override;
        virtual void remove(EntityId entity) override;

    private:
        std::vector<RenderCmd> render_cmds;
    };
}