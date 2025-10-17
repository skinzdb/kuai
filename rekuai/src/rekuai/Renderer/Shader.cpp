#include "Shader.h"

#include "rekuai/Renderer/RendererAPI.h"

#ifdef KU_VULKAN
    #include "rekuai/Platform/Vulkan/VulkanAPI.h"
    #include "rekuai/Platform/Vulkan/VulkanShader.h"
#endif

#ifdef KU_OPENGL
    #include "rekuai/Platform/OpenGL/OpenGLShader.h"
#endif

namespace kuai {
    std::unordered_map<std::string, uint32_t> Shader::ubos{};
	std::unordered_map<std::string, uint32_t> Shader::ubo_offsets{};

    std::unordered_map<uint32_t, std::shared_ptr<Shader>> Shader::shader_map{};

    std::shared_ptr<Shader> Shader::create(const std::string &vert_src, const std::string &frag_src)
    {
        #ifdef KU_VULKAN
            VulkanAPI* vk_api = static_cast<VulkanAPI*>(RendererAPI::get());
            auto shader = std::make_shared<VulkanShader>(vk_api->get_device(), vert_src, frag_src);
            shader_map[shader->get_id()] = shader;
            return shader;
        #endif

        #ifdef KU_OPENGL
            auto shader = std::make_shared<OpenGLShader>(vert_src, frag_src);
            shader_map[shader->get_id()] = shader;
            return shader;
        #endif

        exit(1);
    }

    std::shared_ptr<Shader> Shader::get(uint32_t prog_id)
    {
        return shader_map.at(prog_id);
    }
}
