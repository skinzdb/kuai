#pragma once

#include "vulkan/vulkan_core.h"

namespace kuai {

    struct SpirvReflection {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        std::vector<VkPushConstantRange> push_constants;
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool is_ok() {
            return graphics_family.has_value() && present_family.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    class VulkanUtils
    {
    public:
        static SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

        static QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);

        static uint32_t get_memory_type(VkPhysicalDevice physical_device, uint32_t filter, VkMemoryPropertyFlags properties);
    };
}
