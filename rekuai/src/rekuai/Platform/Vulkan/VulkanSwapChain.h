#pragma once

#include "VulkanFrame.h"

#include "vulkan/vulkan_core.h"

namespace kuai {
    struct SwapChainSupportDetails;

    class VulkanSwapChain
    {
    public:
        VulkanSwapChain(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface);
        
        void draw_frame(VkDevice device, VkPhysicalDevice physical_device, VkQueue graphics_queue, VkQueue present_queue, 
            VkSurfaceKHR surface, const std::vector<VkCommandBuffer>& cmd_bufs,
            std::shared_ptr<VulkanShader> shader, std::shared_ptr<VulkanVertexArray> vertex_array);
            
        size_t get_size() const { return images.size(); }
        
        void cleanup(VkDevice device);

    private:
            
        void create(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface);
        void recreate(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface);
        void create_sync_objects(VkDevice device);

    private:
        VkSwapchainKHR chain;

        VkFormat image_format;
        VkExtent2D extent;

        std::vector<VkImage> images;
        std::vector<std::unique_ptr<VulkanFrame>> frames;

        std::vector<VkSemaphore> img_available_semaphores;
        std::vector<VkSemaphore> render_finished_semaphores;
        std::vector<VkFence> in_flight_fences;

        uint32_t frame_idx;
    };
}
