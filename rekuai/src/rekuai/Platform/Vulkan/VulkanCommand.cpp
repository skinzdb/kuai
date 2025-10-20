#include "VulkanCommand.h"
#include "rekuai/Platform/Vulkan/VulkanBuffer.h"
#include "rekuai/Platform/Vulkan/VulkanUtils.h"
#include "rekuai/Renderer/Buffer.h"
#include "vulkan/vulkan_core.h"
#include <memory>

namespace kuai {

    VulkanCommand::VulkanCommand(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, size_t swap_chain_images)
        : cmd_bufs(std::vector<VkCommandBuffer>(swap_chain_images))
    {
        create_command_pool(device, physical_device, surface);
        create_command_bufs(device);
    }

    void VulkanCommand::create_command_pool(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices queueFamilyIndices = VulkanUtils::find_queue_families(physical_device, surface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphics_family.value();

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &pool) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to create command pool");
            exit(1);
        }
    }

    void VulkanCommand::create_command_bufs(VkDevice device)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = pool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(cmd_bufs.size());

        if (vkAllocateCommandBuffers(device, &allocInfo, cmd_bufs.data()) != VK_SUCCESS) {
            KU_CORE_CRITICAL("(Vulkan) Failed to create command buffers");
            exit(1);
        }
    }

    void VulkanCommand::cleanup(VkDevice device)
    {
        vkDestroyCommandPool(device, pool, nullptr);
    }
}
