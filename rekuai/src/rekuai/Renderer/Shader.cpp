#include "Shader.h"

#ifdef KU_VULKAN
    #include "rekuai/Platform/Vulkan/VulkanShader.h"
#endif 

#ifdef KU_OPENGL
    #include "rekuai/Platform/OpenGL/OpenGLShader.h"
#endif 

#include "rekuai/Renderer/RendererAPI.h"

namespace kuai {
    std::unordered_map<std::string, uint32_t> OpenGLShader::ubos{};
	std::unordered_map<std::string, uint32_t> OpenGLShader::ubo_offsets{};
    
    std::unordered_map<uint32_t, std::shared_ptr<Shader>> Shader::shader_map{};

    std::shared_ptr<Shader> Shader::create(const std::string &vert_src, const std::string &frag_src) 
    {
        #ifdef KU_VULKAN
            auto shader = std::make_shared<VulkanShader>(vert_src, frag_src);
            shader_map[shader->get_id()] = shader;
            return shader;
        #endif 

        #ifdef KU_OPENGL
            auto shader = std::make_shared<OpenGLShader>(vert_src, frag_src);
            shader_map[shader->get_id()] = shader;
            return shader;
        #endif 
       
        return nullptr;
    }

    std::shared_ptr<Shader> Shader::get(uint32_t prog_id)
    {
        return shader_map.at(prog_id);
    }
}
