#pragma once

#include "VulkanBuffer.h"
#include "VulkanShader.h"

#include "vulkan/vulkan_core.h"

namespace kuai 
{

class VulkanFrame
{
public:
    VulkanFrame(VkDevice device, VkImage image, VkFormat swap_chain_format);
    ~VulkanFrame();

    void record(VkDevice device, VkCommandBuffer cmd_buf, VkExtent2D extent, uint32_t index_count, 
        std::shared_ptr<VulkanShader> shader, std::shared_ptr<VulkanVertexArray> vertex_array);

private:

    void create_image_view(VkDevice device, VkFormat swap_chain_format);
    void create_pipeline(VkDevice device, VkExtent2D extent); 

private:
    VkDevice device;

    VkImage image;
    VkImageView image_view;

    VkCommandBuffer cmd_buf;

    friend class VulkanAPI;
};

}
