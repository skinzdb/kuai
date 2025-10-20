#pragma once

#include "rekuai/Renderer/Buffer.h"

#include "vulkan/vulkan_core.h"
#include <optional>
#include <vector>

namespace kuai {

#define GET_EXTENSION_FN(_device, _id) reinterpret_cast<PFN_##_id>(vkGetDeviceProcAddr(_device, #_id))

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
    static SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.present_modes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentModeCount, details.present_modes.data());
        }

        return details;
    }

    static QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport)
            {
                indices.present_family = i;
            }

            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphics_family = i;
            }

            if (indices.is_ok())
            {
                break;
            }
            i++;
        }

        return indices;
    }

    static uint32_t get_memory_type(VkPhysicalDevice physical_device, uint32_t filter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physical_device, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
        {
            if ((filter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
            {
                return i;
            }
        }

        KU_CORE_CRITICAL("(Vulkan) Failed to find suitable memory type");
        exit(1);
    }

    static VkFormat get_vulkan_type(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::INT:
            return VK_FORMAT_R32_SINT;
        case ShaderDataType::FLOAT:
            return VK_FORMAT_R32_SFLOAT;
        case ShaderDataType::VEC2:
            return VK_FORMAT_R32G32_SFLOAT;
        case ShaderDataType::VEC3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case ShaderDataType::VEC4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case ShaderDataType::MAT3:
        case ShaderDataType::MAT4:
            return VK_FORMAT_R32_SFLOAT;
        default:
            KU_CORE_ASSERT(false, "Unknown shader data type");
            return VK_FORMAT_UNDEFINED;
        }
    }
};

}
