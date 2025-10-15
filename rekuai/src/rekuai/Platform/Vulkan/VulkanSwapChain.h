#pragma once

#include "vulkan/vulkan_core.h"

namespace kuai {
    struct SwapChainSupportDetails;

    class VulkanSwapChain
    {
    public:
        void create(VkDevice device, VkPhysicalDevice physical_device);
        void create_image_views(VkDevice device);
        void create_surface(VkInstance instance);
        void create_framebuffers(VkDevice device, VkRenderPass render_pass);
        void recreate(VkDevice device, VkPhysicalDevice physical_device, VkRenderPass render_pass);

        void cleanup(VkDevice device);
        void cleanup_surface(VkInstance instance);

    private:
        VkSurfaceKHR surface;

        VkSwapchainKHR swap_chain;

        VkFormat swap_chain_image_format;
        VkExtent2D swap_chain_extent;

        std::vector<VkImage> swap_chain_images;
        std::vector<VkImageView> swap_chain_image_views;
        std::vector<VkFramebuffer> swap_chain_framebuffers;

        friend class VulkanAPI;
        friend class VulkanCommand;
    };
}
