#pragma once

#include "vulkan/vulkan_core.h"

namespace kuai {
    struct SwapChainSupportDetails;

    class VulkanSwapChain
    {
    public:
        void create(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface);
        void create_image_views(VkDevice device);
        void create_framebuffers(VkDevice device, VkRenderPass render_pass);
        void recreate(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkRenderPass render_pass);

        void cleanup(VkDevice device);

    private:
        VkSwapchainKHR chain;

        VkFormat image_format;
        VkExtent2D extent;

        std::vector<VkImage> images;
        std::vector<VkImageView> image_views;
        std::vector<VkFramebuffer> framebuffers;

        friend class VulkanAPI;
        friend class VulkanCommand;
    };
}
