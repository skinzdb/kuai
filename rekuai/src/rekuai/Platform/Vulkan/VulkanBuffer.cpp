#include "VulkanBuffer.h"

#include "rekuai/Core/Log.h"
#include "rekuai/Platform/Vulkan/VulkanAPI.h"
#include "rekuai/Renderer/RendererAPI.h"
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

    VulkanBuffer::VulkanBuffer(VkDevice device, uint32_t size) : device(device)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buf) != VK_SUCCESS) {
            KU_CORE_ERROR("(Vulkan) Failed to create vertex buffer");
        }
    }

    VulkanBuffer::VulkanBuffer(VkDevice device, const float* vertices, uint32_t size) : device(device)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buf) != VK_SUCCESS) {
            KU_CORE_ERROR("(Vulkan) Failed to create vertex buffer");
        }
    }

    VulkanBuffer::~VulkanBuffer() {
        vkDestroyBuffer(device, buf, nullptr);
        vkFreeMemory(device, buf_memory, nullptr);
    }

    void VulkanBuffer::set_data(const void* data, uint32_t size)
    {
        VkVertexInputBindingDescription binding_description{};
        binding_description.binding = 0;
        binding_description.stride = layout.get_stride();
        binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        std::vector<VkVertexInputAttributeDescription> attr_descriptions;

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

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buf, &memRequirements);

        uint32_t memory_type = static_cast<VulkanAPI*>(RendererAPI::get())->get_memory_type(memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = memory_type;

        if (vkAllocateMemory(device, &allocInfo, nullptr, &buf_memory) != VK_SUCCESS) {
            KU_CORE_ERROR("(Vulkan) Failed to allocate vertex buffer memory");
        }

        vkBindBufferMemory(device, buf, buf_memory, 0);

        void* buf_data;
        vkMapMemory(device, buf_memory, 0, size, 0, &buf_data);
        memcpy(buf_data, data, size);
        vkUnmapMemory(device, buf_memory);

        static_cast<VulkanAPI*>(RendererAPI::get())->add_vertex_buffer(buf, binding_description, attr_descriptions);
    }

    void VulkanBuffer::bind() const
    {

    }

    VulkanVertexArray::VulkanVertexArray(VkDevice device)
    {

    }

    VulkanVertexArray::~VulkanVertexArray()
    {

    }

    void VulkanVertexArray::bind() const
    {

    }

    void VulkanVertexArray::add_vertex_buffer(std::unique_ptr<VertexBuffer> buf)
    {
        vertex_bufs.push_back(std::move(buf));
    }

    void VulkanVertexArray::set_index_buffer(std::unique_ptr<IndexBuffer> buf)
    {
        index_buf = std::move(buf);
    }
}
