#pragma once

#include "rekuai/Renderer/Buffer.h"

namespace kuai {
    class VulkanBuffer : public VertexBuffer {
    public:
        VulkanBuffer(uint32_t size);

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void set_data(const void* data, uint32_t size) override;

        static std::unique_ptr<VertexBuffer> create(uint32_t size);
    };
}
