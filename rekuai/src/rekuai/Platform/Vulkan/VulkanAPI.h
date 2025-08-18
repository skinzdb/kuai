#pragma once

#include <cstdint>
#include <vulkan/vulkan.h>

#include "rekuai/Renderer/RendererAPI.h"
#include "vulkan/vulkan_core.h"

namespace kuai {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool is_ok() {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    class VulkanAPI : public RendererAPI {
    public:
        virtual void init() override;
        virtual void clear() override;

        ~VulkanAPI() override;

    private:
        void create_instance();
        void setup_debug_messenger();
        void create_surface();
        void pick_physical_device();
        void create_logical_device();
        void create_swap_chain();
        void create_image_views();
        void create_graphics_pipeline();
        bool is_device_suitable(const VkPhysicalDevice& device);
        QueueFamilyIndices find_queue_families(const VkPhysicalDevice& device);
        SwapChainSupportDetails query_swap_chain_support(const VkPhysicalDevice& device);

    private:
        VkInstance instance;
        VkDebugUtilsMessengerEXT debug_messenger;
        VkSurfaceKHR surface;
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        VkDevice device;

        VkQueue graphics_queue;
        VkQueue present_queue;

        VkSwapchainKHR swap_chain;
        std::vector<VkImage> swap_chain_images;
        VkFormat swap_chain_image_format;
        VkExtent2D swap_chain_extent;
        std::vector<VkImageView> swap_chain_image_views;
    };

}
