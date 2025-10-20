#include "kpch.h"

#include "VulkanDescriptor.h"

namespace kuai {


    VulkanDescriptor::VulkanDescriptor(VkDevice device)
    {
        VkDescriptorPoolSize poolSizes[] = {
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_DESCRIPTORS },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_DESCRIPTORS }
        };

        VkDescriptorPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        createInfo.maxSets = 1;
        createInfo.poolSizeCount = 2;
        createInfo.pPoolSizes = poolSizes;

        if (vkCreateDescriptorPool(device, &createInfo, nullptr, &pool) != VK_SUCCESS)
        {
            KU_CORE_CRITICAL("(Vulkan) Unable to create descriptor pool");
            exit(1);
        }
    }

VkDescriptorSet allocateVariableDescriptorSet(VkDevice device, VkDescriptorSetLayout layout, uint32_t descriptorCount) {
    // Create a pool that can hold up to 1000 samplers
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize.descriptorCount = 1000;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT; // optional for dynamic updates

    VkDescriptorPool pool;
    vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool);

    // Set variable descriptor count
    VkDescriptorSetVariableDescriptorCountAllocateInfo countInfo{};
    countInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    countInfo.descriptorSetCount = 1;
    countInfo.pDescriptorCounts = &descriptorCount;

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;
    allocInfo.pNext = &countInfo;

    VkDescriptorSet descriptorSet;
    VkResult res = vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet);
    assert(res == VK_SUCCESS);
    return descriptorSet;
}

    VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device) 
    {
        VkDescriptorSetLayoutBinding bindings[2]{};
        bindings[0].binding = 0;
        bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        bindings[0].descriptorCount = MAX_DESCRIPTORS;
        bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        bindings[1].binding = 1;
        bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        bindings[1].descriptorCount = MAX_DESCRIPTORS;
        bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorBindingFlags bindingFlags[] = {
            VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT,
            VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT
        };

        VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsCreateInfo{};
        bindingFlagsCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
        bindingFlagsCreateInfo.pBindingFlags = bindingFlags;
        bindingFlagsCreateInfo.bindingCount = 2;

        VkDescriptorSetLayoutCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.pNext = &bindingFlagsCreateInfo;
        createInfo.bindingCount = 2;
        createInfo.pBindings = bindings;

        VkDescriptorSetLayout layout;
        VkResult res = vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &layout);
        assert(res == VK_SUCCESS);
        return layout;
    }


}
