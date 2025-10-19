#include "VulkanCommand.h"
#include "rekuai/Platform/Vulkan/VulkanBuffer.h"
#include "rekuai/Platform/Vulkan/VulkanUtils.h"
#include "rekuai/Renderer/Buffer.h"
#include "vulkan/vulkan_core.h"
#include <memory>

namespace kuai {

    VulkanCommand::VulkanCommand(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, size_t swap_chain_images) 
    {
        create_command_pool(device, physical_device, surface);
        create_command_buffers(device, swap_chain_images);
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

    void VulkanCommand::record(VkDevice device, std::shared_ptr<VulkanSwapChain> swap_chain,
                    VkRenderPass render_pass, uint32_t current_frame, uint32_t image_idx, uint32_t index_count)
    {
        VkCommandBuffer cmd_buf = cmd_bufs[current_frame];

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(cmd_buf, &beginInfo) != VK_SUCCESS)
        {
            KU_CORE_ERROR("(Vulkan) Failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = render_pass;
        renderPassInfo.framebuffer = swap_chain->framebuffers[image_idx];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swap_chain->extent;

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(cmd_buf, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swap_chain->extent.width);
        viewport.height = static_cast<float>(swap_chain->extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cmd_buf, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swap_chain->extent;
        vkCmdSetScissor(cmd_buf, 0, 1, &scissor);

        shader_bind_fn(cmd_buf);
        vertex_array_bind_fn(cmd_buf);

        vkCmdDrawIndexed(cmd_buf, index_count, 1, 0, 0, 0);

        vkCmdEndRenderPass(cmd_buf);

        if (vkEndCommandBuffer(cmd_buf) != VK_SUCCESS)
        {
            KU_CORE_ERROR("Failed to record command buffer!");
        }
    }

    void VulkanCommand::create_command_buffers(VkDevice device, size_t swap_chain_images)
    {
        cmd_bufs.resize(swap_chain_images);

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
