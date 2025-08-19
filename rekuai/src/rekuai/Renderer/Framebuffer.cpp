#include "Framebuffer.h"
#include "rekuai/Platform/OpenGL/OpenGLFramebuffer.h"
#include "rekuai/Renderer/RendererAPI.h"

namespace kuai {
    std::shared_ptr<Framebuffer> Framebuffer::create(uint32_t width, uint32_t height, uint32_t samples, uint32_t attachments) {
        switch (RendererAPI::getAPI()) {
            case RendererAPI::API::None:
                return nullptr;

            case RendererAPI::API::OpenGL:
                return std::make_unique<OpenGLFramebuffer>(width, height, samples, attachments);

            case RendererAPI::API::Vulkan:
                return nullptr;
        }
    }
}
