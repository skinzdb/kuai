#include "VulkanShader.h"

#include "VulkanAPI.h"
#include "rekuai/Core/Log.h"

namespace kuai {

    VulkanShader::VulkanShader(VkDevice device, const std::string& vert_src, const std::string& frag_src)
        : device(device)
    {
        bind_shaders_fn = (PFN_vkCmdBindShadersEXT) vkGetDeviceProcAddr(device, "vkCmdBindShadersEXT");

        VkShaderCreateInfoEXT vert_info{};
        vert_info.sType = VK_STRUCTURE_TYPE_SHADER_CREATE_INFO_EXT;
        vert_info.flags = VK_SHADER_CREATE_LINK_STAGE_BIT_EXT;
        vert_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vert_info.nextStage = VK_SHADER_STAGE_FRAGMENT_BIT;
        vert_info.codeType = VK_SHADER_CODE_TYPE_SPIRV_EXT;
        vert_info.codeSize = vert_src.size();
        vert_info.pCode = vert_src.data();
        vert_info.pName = "main";


        VkShaderCreateInfoEXT frag_info{};
        frag_info.sType = VK_STRUCTURE_TYPE_SHADER_CREATE_INFO_EXT;
        frag_info.flags = VK_SHADER_CREATE_LINK_STAGE_BIT_EXT;
        frag_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        frag_info.codeType = VK_SHADER_CODE_TYPE_SPIRV_EXT;
        frag_info.codeSize = frag_src.size();
        frag_info.pCode = frag_src.data();
        frag_info.pName = "main";

        VkShaderCreateInfoEXT shader_infos[2] = {vert_info, frag_info}; 

        auto create_shaders = (PFN_vkCreateShadersEXT) vkGetDeviceProcAddr(device, "vkCreateShadersEXT");

        if (create_shaders(device, 2, shader_infos, nullptr, shaders) != VK_SUCCESS)
        {
            KU_CORE_ERROR("(Vulkan) Failed to create shader object");
        }
    }

    VulkanShader::~VulkanShader()
    {
        auto destroy_shader = (PFN_vkDestroyShaderEXT) vkGetDeviceProcAddr(device, "vkDestroyShaderEXT");
        destroy_shader(device, shaders[0], nullptr);
        destroy_shader(device, shaders[1], nullptr);
    }

    void VulkanShader::create_uniform(const std::string& name) 
    {

    }
    
	void VulkanShader::set_uniform(const std::string& name, int val) const {

	}
	void VulkanShader::set_uniform(const std::string& name, float val) const {

	}
	void VulkanShader::set_uniform(const std::string& name, const glm::vec2& val) const {

	}
	void VulkanShader::set_uniform(const std::string& name, const glm::vec3& val) const {

	}
	void VulkanShader::set_uniform(const std::string& name, const glm::vec4& val) const {

	}
	void VulkanShader::set_uniform(const std::string& name, const glm::mat3& val) const {

	}
	void VulkanShader::set_uniform(const std::string& name, const glm::mat4& val) const {

	}

    void VulkanShader::create_uniform_block(const std::string &name, const std::vector<const char *> &members, uint32_t binding)
    {

    }

    void VulkanShader::set_uniform_block(const std::string &name, const std::string &member, const void *data, uint32_t size)
    {

    }

    void VulkanShader::bind() const 
    {
        VulkanAPI* vk_api = static_cast<VulkanAPI*>(RendererAPI::get());

        vk_api->set_shader_bind_fn([this](VkCommandBuffer cmd_buf) {
            VkShaderStageFlagBits shader_stages[2] = { VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT };
            bind_shaders_fn(cmd_buf, 2, shader_stages, shaders);
        });
	}

	void VulkanShader::unbind() const {

	}

}
