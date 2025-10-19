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

        void record(VkDevice device, std::shared_ptr<VulkanSwapChain> swap_chain,
                    VkRenderPass render_pass, uint32_t current_frame, uint32_t image_idx, uint32_t index_count);

        void cleanup(VkDevice device);

        void set_shader_bind_fn(const std::function<void(VkCommandBuffer)>& fn) { shader_bind_fn = fn; }
        void set_vertex_array_bind_fn(const std::function<void(VkCommandBuffer)>& fn) { vertex_array_bind_fn = fn; }

    private:
        void create_command_pool(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface);
        void create_command_buffers(VkDevice device, size_t swap_chain_images);

    private:
        VkCommandPool pool;
        std::vector<VkCommandBuffer> cmd_bufs;

        std::function<void(VkCommandBuffer)> shader_bind_fn;
        std::function<void(VkCommandBuffer)> vertex_array_bind_fn;

        friend class VulkanAPI;
    };
}
