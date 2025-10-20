#pragma once

#include "rekuai/Platform/Vulkan/VulkanShader.h"
#include "rekuai/Platform/Vulkan/VulkanSwapChain.h"
#include "rekuai/Renderer/Buffer.h"

#include "vulkan/vulkan_core.h"
#include <functional>

namespace kuai {

    class VulkanCommand
    {
    public:
        VulkanCommand(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, size_t swap_chain_images);

        void cleanup(VkDevice device);
        
        std::vector<VkCommandBuffer>& get_command_buffers() { return cmd_bufs; }

    private:
        void create_command_pool(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface);
        void create_command_bufs(VkDevice device);

    private:
        VkCommandPool pool;

        std::vector<VkCommandBuffer> cmd_bufs;

        friend class VulkanAPI;
    };
}
