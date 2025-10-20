#include "kpch.h"

#include "VulkanFrame.h"
#include "VulkanUtils.h"

namespace kuai {

    VulkanFrame::VulkanFrame(VkDevice device, VkImage image, VkFormat swap_chain_format) 
        : device(device), image(image)
    {   
        create_image_view(device, swap_chain_format);
    }

    VulkanFrame::~VulkanFrame()
    {
        vkDestroyImageView(device, image_view, nullptr);
    }

    void VulkanFrame::record(VkDevice device, VkCommandBuffer cmd_buf, VkExtent2D extent, uint32_t index_count, 
        std::shared_ptr<VulkanShader> shader, std::shared_ptr<VulkanVertexArray> vertex_array)
    {
        this->cmd_buf = cmd_buf;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(cmd_buf, &beginInfo);
        
        VkImageMemoryBarrier beforeBarrier{};
        beforeBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        beforeBarrier.srcAccessMask = VK_ACCESS_NONE;
        beforeBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        beforeBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        beforeBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        beforeBarrier.image = image;
        beforeBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        beforeBarrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
        beforeBarrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

        vkCmdPipelineBarrier(
            cmd_buf,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &beforeBarrier
        );

        VkRenderingAttachmentInfo colour_attachment_info{};
        colour_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        colour_attachment_info.imageView = image_view;
        colour_attachment_info.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colour_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colour_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colour_attachment_info.clearValue.color = { { 0.1f, 0.1f, 0.1f, 1.0f } };

        VkRenderingInfo rendering_info{};
        rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        rendering_info.renderArea = { { 0, 0 }, { extent.width, extent.height } };
        rendering_info.layerCount = 1;
        rendering_info.colorAttachmentCount = 1;
        rendering_info.viewMask = 0;
        rendering_info.pColorAttachments = &colour_attachment_info;

        vkCmdBeginRendering(cmd_buf, &rendering_info);      
        
        create_pipeline(device, extent);
        
        shader->bind(cmd_buf);

        vertex_array->bind(cmd_buf);

        vkCmdDrawIndexed(cmd_buf, index_count, 1, 0, 0, 0);

        vkCmdEndRendering(cmd_buf);

        VkImageMemoryBarrier afterBarrier{};
        afterBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        afterBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        afterBarrier.dstAccessMask = VK_ACCESS_NONE;
        afterBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        afterBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        afterBarrier.image = image;
        afterBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        afterBarrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
        afterBarrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

        vkCmdPipelineBarrier(
            cmd_buf,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &afterBarrier
        );

        vkEndCommandBuffer(cmd_buf);
    }

    void VulkanFrame::create_image_view(VkDevice device, VkFormat swap_chain_format)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = image;

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swap_chain_format;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, nullptr, &image_view) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to create image view");
            exit(1);
        }
    }

    void VulkanFrame::create_pipeline(VkDevice device, VkExtent2D extent)
    {
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extent.width);
        viewport.height = static_cast<float>(extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewportWithCount(cmd_buf, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = extent;
        vkCmdSetScissorWithCount(cmd_buf, 1, &scissor);

        // vkCmdBindDescriptorSets(
        //     cmd_buf,
        //     VK_PIPELINE_BIND_POINT_GRAPHICS,
        //     ctx->pipelineLayout,
        //     0,
        //     1,
        //     &descriptorSet,
        //     0,
        //     nullptr
        // );

        // Input assembly settings
        vkCmdSetPrimitiveTopology(cmd_buf, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
        vkCmdSetPrimitiveRestartEnable(cmd_buf, false);

        // Rasterization settings
        vkCmdSetRasterizerDiscardEnable(cmd_buf, false);
        vkCmdSetCullMode(cmd_buf, VK_CULL_MODE_NONE);
        GET_EXTENSION_FN(device, vkCmdSetPolygonModeEXT)(cmd_buf, VK_POLYGON_MODE_FILL);
        vkCmdSetDepthBiasEnable(cmd_buf, false);

        // Multisample settings
        GET_EXTENSION_FN(device, vkCmdSetRasterizationSamplesEXT)(cmd_buf, VK_SAMPLE_COUNT_1_BIT);

        VkSampleMask sampleMask = 1;
        GET_EXTENSION_FN(device, vkCmdSetSampleMaskEXT)(cmd_buf, VK_SAMPLE_COUNT_1_BIT, &sampleMask);
        GET_EXTENSION_FN(device, vkCmdSetAlphaToCoverageEnableEXT)(cmd_buf, false);

        // Depth stencil stetings
        vkCmdSetDepthWriteEnable(cmd_buf, false);
        vkCmdSetDepthTestEnable(cmd_buf, false);
        vkCmdSetStencilTestEnable(cmd_buf, false);

        // Color blend settings
        VkBool32 colorBlend = true;
        GET_EXTENSION_FN(device, vkCmdSetColorBlendEnableEXT)(cmd_buf, 0, 1, &colorBlend);
        VkColorBlendEquationEXT colorBlendEquation{};
        colorBlendEquation.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendEquation.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendEquation.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendEquation.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendEquation.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendEquation.alphaBlendOp = VK_BLEND_OP_ADD;
        GET_EXTENSION_FN(device, vkCmdSetColorBlendEquationEXT)(cmd_buf, 0, 1, &colorBlendEquation);
        VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        GET_EXTENSION_FN(device, vkCmdSetColorWriteMaskEXT)(cmd_buf, 0, 1, &colorWriteMask);
    }
}
