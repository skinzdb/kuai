#pragma once

#include "rekuai/Renderer/Buffer.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>

namespace kuai {
    class VulkanBuffer : public VertexBuffer
    {
    public:
        VulkanBuffer(VkDevice device, VkPhysicalDevice physical_device, VkCommandPool command_pool,
           VkQueue graphics_queue, uint32_t size);
        ~VulkanBuffer();

        virtual void bind() const override {}
        void bind(VkCommandBuffer cmd_buf) const;

        virtual void set_data(const void* data, uint32_t size) override;

        virtual BufferLayout& get_layout() override { return layout; }
        virtual void set_layout(const BufferLayout& layout) override;

    private:
        BufferLayout layout;

        VkBuffer buf;
        VkDeviceMemory buf_memory;

        VkVertexInputBindingDescription2EXT binding_description;
        std::vector<VkVertexInputAttributeDescription2EXT> attr_descriptions;

        VkDevice ctx_device;
        VkPhysicalDevice ctx_physical_device;
        VkCommandPool command_pool;
        VkQueue graphics_queue; // TODO: clean, bad

        friend class VulkanVertexArray;
    };

    class VulkanIndexBuffer : public IndexBuffer
    {
    public:
        VulkanIndexBuffer(VkDevice device, VkPhysicalDevice physical_device, VkCommandPool command_pool,
            VkQueue graphics_queue, const uint32_t* indices, uint32_t count);
        ~VulkanIndexBuffer();

        virtual void bind() const override {}
        void bind(VkCommandBuffer cmd_buf) const;

        virtual uint32_t get_count() const override { return count; }

    private:
        VkBuffer buf;
        VkDeviceMemory buf_memory;

        uint32_t count;

        VkDevice ctx_device;

        friend class VulkanVertexArray;
    };

    class VulkanVertexArray : public VertexArray
    {
    public:
        virtual void bind() const override {}
        void bind(VkCommandBuffer cmd_buf) const;

        virtual void add_vertex_buffer(std::shared_ptr<VertexBuffer> buf) override;
        virtual void set_index_buffer(std::shared_ptr<IndexBuffer> buf) override;

        virtual uint32_t get_index_count() const override { return index_buf->get_count(); }

        virtual uint32_t get_id() const override { return 0; }

    private:
        std::vector<std::shared_ptr<VulkanBuffer>> vertex_bufs;
        std::vector<VkBuffer> vk_vertex_bufs;

        std::shared_ptr<VulkanIndexBuffer> index_buf;

        friend class VulkanCommand;
    };
}
