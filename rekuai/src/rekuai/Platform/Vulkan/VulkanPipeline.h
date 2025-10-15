#pragma once

#include "rekuai/Renderer/Buffer.h"
#include "vulkan/vulkan_core.h"

namespace kuai {
    class VulkanPipeline
    {
    public:
        VulkanPipeline();

        void create(VkDevice device, VkRenderPass render_pass);
        void cleanup(VkDevice device);

        void set_shaders(VkShaderModule vert, VkShaderModule frag);
        void set_vertex_layout(const BufferLayout& layout);

    private:
        VkPipelineLayout pipeline_layout;
        VkPipeline graphics_pipeline;

        VkPipelineShaderStageCreateInfo shader_stages[2];
        VkPipelineVertexInputStateCreateInfo vertex_input_info;

        VkVertexInputBindingDescription binding_description;
        std::vector<VkVertexInputAttributeDescription> attr_descriptions;

        friend class VulkanCommand;
    };
}
