#pragma once

#include "rekuai/Renderer/Buffer.h"
#include "vulkan/vulkan_core.h"

namespace kuai {
    class VulkanBuffer : public VertexBuffer
    {
    public:
        VulkanBuffer(VkDevice device, uint32_t size);
        VulkanBuffer(VkDevice device, const float* vertices, uint32_t size);
        ~VulkanBuffer();

        virtual void bind() const override;

        virtual void set_data(const void* data, uint32_t size) override;

        virtual BufferLayout& get_layout() override { return layout; }
        virtual void set_layout(const BufferLayout& layout) override { this->layout = layout; }

    private:
        BufferLayout layout;

        VkBuffer buf;
        VkDeviceMemory buf_memory;
        VkDevice device;
    };

    class VulkanVertexArray : public VertexArray
    {
    public:
        VulkanVertexArray(VkDevice device);
        ~VulkanVertexArray();

        virtual void bind() const;

        virtual void add_vertex_buffer(std::unique_ptr<VertexBuffer> buf);
        virtual void set_index_buffer(std::unique_ptr<IndexBuffer> buf);

        virtual uint32_t get_index_count() const { return index_buf->get_count(); }

    private:
        std::vector<std::unique_ptr<VertexBuffer>> vertex_bufs;
        std::unique_ptr<IndexBuffer> index_buf;
    };
}
