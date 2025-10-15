#include "Buffer.h"

#ifdef KU_VULKAN
    #include "rekuai/Platform/Vulkan/VulkanAPI.h"
    #include "rekuai/Platform/Vulkan/VulkanBuffer.h"
#endif

#ifdef KU_OPENGL
    #include "rekuai/Platform/OpenGL/OpenGLBuffer.h"
#endif

namespace kuai {

	// Vertex Buffer *********************************************************

    std::shared_ptr<VertexBuffer> VertexBuffer::create(uint32_t size)
    {
        #ifdef KU_VULKAN
            auto vk_api = static_cast<VulkanAPI*>(RendererAPI::get());
            return std::make_shared<VulkanBuffer>(vk_api->get_device(), vk_api->get_physical_device(),
                vk_api->get_command_pool(), vk_api->get_graphics_queue(), size);
        #endif

        #ifdef KU_OPENGL
            return std::make_shared<OpenGLBuffer>(size);
        #endif

        return nullptr;
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::create(const uint32_t* indices, uint32_t count)
    {
        #ifdef KU_VULKAN
            auto vk_api = static_cast<VulkanAPI*>(RendererAPI::get());
            return std::make_shared<VulkanIndexBuffer>(vk_api->get_device(), vk_api->get_physical_device(),
                vk_api->get_command_pool(), vk_api->get_graphics_queue(), indices, count);
        #endif

        #ifdef KU_OPENGL
            return std::make_unique<OpenGLIndexBuffer>(indices, count);
        #endif

        return nullptr;
    }

    std::shared_ptr<VertexArray> VertexArray::create()
    {
        #ifdef KU_VULKAN
            return std::make_shared<VulkanVertexArray>();
        #endif

        #ifdef KU_OPENGL
            return std::make_shared<OpenGLVertexArray>();
        #endif

        return nullptr;
    }

    std::unique_ptr<IndirectBuffer> IndirectBuffer::create(const std::vector<IndirectCommand>& commands)
    {
        #ifdef KU_VULKAN
            KU_CORE_ERROR("Vulkan IndirectBuffer NOT IMPLEMENTED!");
            // return std::make_unique<VulkanIndirectBuffer>(commands);
        #endif

        #ifdef KU_OPENGL
            return std::make_unique<OpenGLIndirectBuffer>(commands);
        #endif

        return nullptr;
    }
}
