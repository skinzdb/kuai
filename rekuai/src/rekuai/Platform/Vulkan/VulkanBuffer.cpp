#include "kpch.h"

#include "VulkanBuffer.h"

#include "rekuai/Platform/Vulkan/VulkanUtils.h"
#include "VulkanAPI.h"

namespace kuai {

    // Helper function to build buffers
    void create_buffer(VkDevice device, VkPhysicalDevice physical_device, VkDeviceSize size,
        VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
        VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            KU_CORE_ERROR("(Vulkan) Failed to create buffer");
            exit(1);
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VulkanUtils::get_memory_type(physical_device, memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            KU_CORE_ERROR("(Vulkan) Failed to allocate buffer memory");
            exit(1);
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    void copy_buffer(VkDevice device, VkCommandPool command_pool, VkQueue graphics_queue,
        VkBuffer src, VkBuffer dst, VkDeviceSize size)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = command_pool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphics_queue);

        vkFreeCommandBuffers(device, command_pool, 1, &commandBuffer);
    }

    VulkanBuffer::VulkanBuffer(VkDevice device, VkPhysicalDevice physical_device, VkCommandPool command_pool,
        VkQueue graphics_queue, uint32_t size)
        : ctx_device(device), ctx_physical_device(physical_device), command_pool(command_pool), graphics_queue(graphics_queue)
    {
        create_buffer(device, physical_device, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buf, buf_memory);
    }

    VulkanBuffer::~VulkanBuffer() {
        vkDestroyBuffer(ctx_device, buf, nullptr);
        vkFreeMemory(ctx_device, buf_memory, nullptr);
    }

    void VulkanBuffer::set_data(const void* data, uint32_t size)
    {
        VkBuffer staging_buf;
        VkDeviceMemory staging_buf_memory;
        create_buffer(ctx_device, ctx_physical_device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buf, staging_buf_memory);

        void* tmp_data;
        vkMapMemory(ctx_device, staging_buf_memory, 0, size, 0, &tmp_data);
        memcpy(tmp_data, data, (size_t) size);
        vkUnmapMemory(ctx_device, staging_buf_memory);

        copy_buffer(ctx_device, command_pool, graphics_queue, staging_buf, buf, size);

        vkDestroyBuffer(ctx_device, staging_buf, nullptr);
        vkFreeMemory(ctx_device, staging_buf_memory, nullptr);
    }

    VulkanIndexBuffer::VulkanIndexBuffer(VkDevice device, VkPhysicalDevice physical_device, VkCommandPool command_pool,
        VkQueue graphics_queue, const uint32_t* indices, uint32_t count)
        : ctx_device(device), count(count)
    {
        VkDeviceSize buf_size = sizeof(uint32_t) * count;

        VkBuffer staging_buf;
        VkDeviceMemory staging_buf_memory;
        create_buffer(device, physical_device, buf_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buf, staging_buf_memory);

        void* data;
        vkMapMemory(device, staging_buf_memory, 0, buf_size, 0, &data);
        memcpy(data, &indices[0], (size_t) buf_size);
        vkUnmapMemory(device, staging_buf_memory);

        create_buffer(device, physical_device, buf_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buf, buf_memory);

        copy_buffer(device, command_pool, graphics_queue, staging_buf, buf, buf_size);

        vkDestroyBuffer(device, staging_buf, nullptr);
        vkFreeMemory(device, staging_buf_memory, nullptr);
    }

    VulkanIndexBuffer::~VulkanIndexBuffer()
    {
        vkDestroyBuffer(ctx_device, buf, nullptr);
        vkFreeMemory(ctx_device, buf_memory, nullptr);
    }

    void VulkanVertexArray::bind() const
    {
        VulkanAPI* vk_api = static_cast<VulkanAPI*>(RendererAPI::get());

        vk_api->set_vertex_array_bind_fn([this](VkCommandBuffer cmd_buf) {
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(cmd_buf, 0, vk_vertex_bufs.size(), vk_vertex_bufs.data(), offsets);
            vkCmdBindIndexBuffer(cmd_buf, index_buf->buf, 0, VK_INDEX_TYPE_UINT32);
        });
    }

    void VulkanVertexArray::add_vertex_buffer(std::shared_ptr<VertexBuffer> buf)
    {
        auto vk_buf = std::static_pointer_cast<VulkanBuffer>(buf);
        vertex_bufs.push_back(vk_buf);
        vk_vertex_bufs.push_back(vk_buf->buf);
    }

    void VulkanVertexArray::set_index_buffer(std::shared_ptr<IndexBuffer> buf)
    {
        index_buf = std::static_pointer_cast<VulkanIndexBuffer>(buf);
    }
}
