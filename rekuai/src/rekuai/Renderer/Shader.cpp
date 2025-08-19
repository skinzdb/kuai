#include "Shader.h"
#include "rekuai/Platform/OpenGL/OpenGLShader.h"
#include "rekuai/Platform/Vulkan/VulkanShader.h"
#include "rekuai/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace kuai {
    std::unique_ptr<Shader> Shader::create(const std::string &vert_src, const std::string &frag_src) {
        switch (RendererAPI::getAPI()) {
            case RendererAPI::API::None:
                return nullptr;

            case RendererAPI::API::OpenGL:
                return std::make_unique<OpenGLShader>(vert_src, frag_src);

            case RendererAPI::API::Vulkan:
                return std::make_unique<VulkanShader>(vert_src, frag_src);
        }
    }
}
