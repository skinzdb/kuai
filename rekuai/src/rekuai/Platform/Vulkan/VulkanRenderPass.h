#pragma once

#include "vulkan/vulkan_core.h"

namespace kuai {
    class VulkanRenderPass
    {
    public:
        VulkanRenderPass(VkDevice device, VkFormat image_format);

        void cleanup(VkDevice device);

    private:
        VkRenderPass render_pass;

        friend class VulkanAPI;
    };
}
