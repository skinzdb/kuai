#include "VulkanPipeline.h"
#include "rekuai/Core/Log.h"
#include "rekuai/Renderer/Buffer.h"
#include "vulkan/vulkan_core.h"

namespace kuai {
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

	VulkanPipeline::VulkanPipeline()
	{
        vertex_input_info = VkPipelineVertexInputStateCreateInfo{};
        vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_info.vertexBindingDescriptionCount = 0;
        vertex_input_info.vertexAttributeDescriptionCount = 0;
	}

    void VulkanPipeline::set_shaders(VkShaderModule vert, VkShaderModule frag)
    {
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vert;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = frag;
        fragShaderStageInfo.pName = "main";

        shader_stages[0] = vertShaderStageInfo;
        shader_stages[1] = fragShaderStageInfo;
    }

    void VulkanPipeline::set_vertex_layout(const BufferLayout& layout)
    {
        attr_descriptions.clear();

        binding_description = VkVertexInputBindingDescription{};
        binding_description.binding = 0;
        binding_description.stride = layout.get_stride();
        binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        size_t i = 0;
        for (const auto& element : layout)
        {
            VkVertexInputAttributeDescription attr{};
            attr.binding = 0;
            attr.location = static_cast<uint32_t>(i);
            attr.format = get_vulkan_type(element.type);
            attr.offset = element.offset;
            attr_descriptions.push_back(attr);
            i++;
        }

        vertex_input_info = VkPipelineVertexInputStateCreateInfo{};
        vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_info.vertexBindingDescriptionCount = 1;
        vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attr_descriptions.size());
        vertex_input_info.pVertexBindingDescriptions = &binding_description;
        vertex_input_info.pVertexAttributeDescriptions = attr_descriptions.data();
    }

    void VulkanPipeline::create(VkDevice device, VkRenderPass render_pass)
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipeline_layout) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to create pipeline layout");
            exit(1);
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shader_stages;
        pipelineInfo.pVertexInputState = &vertex_input_info;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = pipeline_layout;
        pipelineInfo.renderPass = render_pass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphics_pipeline) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Failed to create graphics pipeline");
            exit(1);
        }
    }

    void VulkanPipeline::cleanup(VkDevice device)
    {
        vkDestroyPipeline(device, graphics_pipeline, nullptr);
        vkDestroyPipelineLayout(device, pipeline_layout, nullptr);
    }
}
