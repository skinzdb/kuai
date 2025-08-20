#include "Shader.h"
#include "rekuai/Platform/OpenGL/OpenGLShader.h"
#include "rekuai/Platform/Vulkan/VulkanShader.h"
#include "rekuai/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace kuai {
    auto Shader::shader_map = std::unordered_map<uint32_t, std::shared_ptr<Shader>>();

    std::shared_ptr<Shader> Shader::create(const std::string &vert_src, const std::string &frag_src) {
        switch (RendererAPI::getAPI()) {
            case RendererAPI::API::None:
                return nullptr;

            case RendererAPI::API::OpenGL: {
                auto shader = std::make_shared<OpenGLShader>(vert_src, frag_src);
                shader_map[shader->get_id()] = shader;
                return shader;
            }
            case RendererAPI::API::Vulkan: {
                auto shader = std::make_shared<VulkanShader>(vert_src, frag_src);
                shader_map[shader->get_id()] = shader;
                return shader;
            }
        }
    }

    std::shared_ptr<Shader> Shader::get(uint32_t id) {
        try {
            auto shader = shader_map.at(id);
            return shader;
        } catch (std::out_of_range e) {
            return nullptr;
        }
    }
}
