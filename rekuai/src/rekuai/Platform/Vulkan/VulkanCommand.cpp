#include "VulkanCommand.h"
#include "rekuai/Platform/Vulkan/VulkanBuffer.h"
#include "rekuai/Platform/Vulkan/VulkanUtils.h"
#include "rekuai/Renderer/Buffer.h"
#include "vulkan/vulkan_core.h"
#include <memory>

namespace kuai {

    VulkanCommand::VulkanCommand(VkDevice device, VkPhysicalDevice physical_device, std::shared_ptr<VulkanSwapChain> swap_chain)
    {
        create_command_pool(device, physical_device, swap_chain->surface);
        create_command_buffers(device, swap_chain);
    }

    void VulkanCommand::create_command_pool(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices queueFamilyIndices = VulkanUtils::find_queue_families(physical_device, surface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphics_family.value();

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &command_pool) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to create command pool");
            exit(1);
        }
    }

    void VulkanCommand::record(VkDevice device, std::shared_ptr<VulkanSwapChain> swap_chain,
        std::shared_ptr<VulkanPipeline> pipeline, VkRenderPass render_pass, VkCommandBuffer cmd_buf,
        uint32_t image_idx, std::shared_ptr<VertexArray> vertex_array)
    {
        auto vk_vertex_array = static_cast<VulkanVertexArray*>(vertex_array.get());

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
        renderPassInfo.framebuffer = swap_chain->swap_chain_framebuffers[image_idx];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swap_chain->swap_chain_extent;

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(cmd_buf, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(cmd_buf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphics_pipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swap_chain->swap_chain_extent.width);
        viewport.height = static_cast<float>(swap_chain->swap_chain_extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cmd_buf, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swap_chain->swap_chain_extent;
        vkCmdSetScissor(cmd_buf, 0, 1, &scissor);

        vk_vertex_array->bind(cmd_buf);

        vkCmdDrawIndexed(cmd_buf, static_cast<uint32_t>(vertex_array->get_index_count()), 1, 0, 0, 0);

        vkCmdEndRenderPass(cmd_buf);

        if (vkEndCommandBuffer(cmd_buf) != VK_SUCCESS)
        {
            KU_CORE_ERROR("Failed to record command buffer!");
        }
    }

    void VulkanCommand::create_command_buffers(VkDevice device, std::shared_ptr<VulkanSwapChain> swap_chain)
    {
        command_bufs.resize(swap_chain->swap_chain_images.size());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = command_pool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(command_bufs.size());

        if (vkAllocateCommandBuffers(device, &allocInfo, command_bufs.data()) != VK_SUCCESS) {
            KU_CORE_CRITICAL("(Vulkan) Failed to create command buffers");
            exit(1);
        }
    }

    void VulkanCommand::cleanup(VkDevice device)
    {
        vkDestroyCommandPool(device, command_pool, nullptr);
    }

}
