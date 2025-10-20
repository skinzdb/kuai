#pragma once

#include "vulkan/vulkan_core.h"

namespace kuai {

    const uint32_t MAX_DESCRIPTORS = 1000;

    class VulkanDescriptor 
    {
    public:
        VulkanDescriptor(VkDevice device);

    private:
        VkDescriptorPool pool;

    };

}