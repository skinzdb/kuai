#pragma once

#include "rekuai/Renderer/Buffer.h"
#include "rekuai/Renderer/RendererAPI.h"
#include <cstdint>

namespace kuai {
    class OpenGLAPI : public RendererAPI {
    public:
        virtual void init() override;
        virtual void stop() override;
        virtual void clear() override;

        virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        virtual void draw_indexed(std::shared_ptr<VertexArray> vertex_array, uint32_t index_count) override;
        // virtual void draw_indirect() override;

        ~OpenGLAPI();
    };
}
