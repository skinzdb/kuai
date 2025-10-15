#pragma once

#include "rekuai/Platform/Vulkan/VulkanPipeline.h"
#include "rekuai/Platform/Vulkan/VulkanSwapChain.h"
#include "rekuai/Renderer/Buffer.h"
#include "vulkan/vulkan_core.h"

namespace kuai {

    class VulkanCommand
    {
    public:
        VulkanCommand(VkDevice device, VkPhysicalDevice physical_device, std::shared_ptr<VulkanSwapChain> swap_chain);

        void record(VkDevice device, std::shared_ptr<VulkanSwapChain> swap_chain,
            std::shared_ptr<VulkanPipeline> pipeline,
            VkRenderPass render_pass, VkCommandBuffer buf, uint32_t image_idx,
            std::shared_ptr<VertexArray> vertex_array);

        void cleanup(VkDevice device);

    private:
        void create_command_pool(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface);
        void create_command_buffers(VkDevice device, std::shared_ptr<VulkanSwapChain> swap_chain);

    private:
        VkCommandPool command_pool;
        std::vector<VkCommandBuffer> command_bufs;

        friend class VulkanAPI;
    };
}
