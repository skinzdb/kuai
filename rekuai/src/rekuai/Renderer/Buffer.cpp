#include "Buffer.h"

#ifdef KU_VULKAN
    #include "rekuai/Platform/Vulkan/VulkanBuffer.h"
#endif

#ifdef KU_OPENGL
    #include "rekuai/Platform/OpenGL/OpenGLBuffer.h"
#endif

namespace kuai {

	// Vertex Buffer *********************************************************

    std::unique_ptr<VertexBuffer> VertexBuffer::create(uint32_t size)
    {
        #ifdef KU_VULKAN
            KU_CORE_ERROR("Vulkan VertexBuffer NOT IMPLEMENTED!");
            return std::make_unique<VulkanBuffer>(size);
        #endif

        #ifdef KU_OPENGL
            return std::make_unique<OpenGLBuffer>(size);
        #endif

        return nullptr;
    }

    std::unique_ptr<VertexBuffer> VertexBuffer::create(const float* vertices, uint32_t size)
    {
        #ifdef KU_VULKAN
            KU_CORE_ERROR("Vulkan VertexBuffer NOT IMPLEMENTED!");
            return std::make_unique<VulkanBuffer>(size);
        #endif

        #ifdef KU_OPENGL
            return std::make_unique<OpenGLBuffer>(vertices, size);
        #endif

        return nullptr;
    }

    std::unique_ptr<IndexBuffer> IndexBuffer::create(const uint32_t* indices, uint32_t count)
    {
        #ifdef KU_VULKAN
            KU_CORE_ERROR("Vulkan IndexBuffer NOT IMPLEMENTED!");
            // return std::make_unique<VulkanIndexBuffer>(indices, count);
        #endif

        #ifdef KU_OPENGL
            return std::make_unique<OpenGLIndexBuffer>(indices, count);
        #endif

        return nullptr;
    }

    std::unique_ptr<VertexArray> VertexArray::create()
    {
        #ifdef KU_VULKAN
            KU_CORE_ERROR("Vulkan VertexArray NOT IMPLEMENTED!");
            // return std::make_unique<VulkanVertexArray>();
        #endif

        #ifdef KU_OPENGL
            return std::make_unique<OpenGLVertexArray>();
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
