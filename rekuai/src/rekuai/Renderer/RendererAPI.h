#pragma once

#include "rekuai/Renderer/Buffer.h"
namespace kuai {
    class RendererAPI
    {
    public:
        enum class API {
            None = 0,
            OpenGL = 1,
            Vulkan = 2
        };

    	virtual ~RendererAPI() = default;

        virtual void init() = 0;
        virtual void clear() = 0;

        virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count) = 0;

        static API getAPI() { return api; }
        static std::unique_ptr<RendererAPI> create();
    private:
        static API api;
    };
}
