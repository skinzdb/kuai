#include "Buffer.h"

#include "rekuai/Platform/OpenGL/OpenGLBuffer.h"
#include "rekuai/Platform/Vulkan/VulkanBuffer.h"
#include "rekuai/Renderer/RendererAPI.h"

namespace kuai {

	// Vertex Buffer *********************************************************

    std::unique_ptr<VertexBuffer> VertexBuffer::create(uint32_t size) {
        switch (RendererAPI::getAPI()) {

        case RendererAPI::API::None:
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_unique<OpenGLBuffer>(size);
        case RendererAPI::API::Vulkan:
            return std::make_unique<VulkanBuffer>(size);
        }
    }

    std::unique_ptr<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size) {
        switch (RendererAPI::getAPI()) {

        case RendererAPI::API::None:
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_unique<OpenGLBuffer>(vertices, size);
        case RendererAPI::API::Vulkan:
            return nullptr;
        }
    }

    std::unique_ptr<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t count) {
        switch (RendererAPI::getAPI()) {

        case RendererAPI::API::None:
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_unique<OpenGLIndexBuffer>(indices, count);
        case RendererAPI::API::Vulkan:
            return nullptr;
        }
    }

    std::unique_ptr<VertexArray> VertexArray::create() {
        switch (RendererAPI::getAPI()) {

        case RendererAPI::API::None:
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_unique<OpenGLVertexArray>();
        case RendererAPI::API::Vulkan:
            return nullptr;
        }
    }

    std::unique_ptr<IndirectBuffer> IndirectBuffer::create(const std::vector<IndirectCommand>& commands) {
        switch (RendererAPI::getAPI()) {

        case RendererAPI::API::None:
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_unique<OpenGLIndirectBuffer>(commands);
        case RendererAPI::API::Vulkan:
            return nullptr;
        }
    }
}
