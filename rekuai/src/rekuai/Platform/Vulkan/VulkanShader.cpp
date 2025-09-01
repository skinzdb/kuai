#include "VulkanShader.h"
#include "VulkanAPI.h"
#include "rekuai/Core/Log.h"
#include "rekuai/Renderer/RendererAPI.h"

namespace kuai {

    VkShaderModule VulkanShader::create_shader_module(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            KU_CORE_ERROR("(Vulkan) Failed to create shader module");
        }

        return shaderModule;
    }

    VulkanShader::VulkanShader(VkDevice device, const std::string& vert_src, const std::string& frag_src) : device(device)
    {
        vert = create_shader_module(std::vector<char>(vert_src.begin(), vert_src.end()));
        frag = create_shader_module(std::vector<char>(frag_src.begin(), frag_src.end()));

        static_cast<VulkanAPI*>(RendererAPI::get())->set_shader_modules(vert, frag);

        // vkDestroyShaderModule(device, vert, nullptr);
        // vkDestroyShaderModule(device, frag, nullptr);
    }

    VulkanShader::~VulkanShader()
    {
        // Shouldn't be here
        vkDestroyShaderModule(device, vert, nullptr);
        vkDestroyShaderModule(device, frag, nullptr);
    }

    void VulkanShader::create_uniform(const std::string& name) {

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

    void VulkanShader::bind() const {

	}

	void VulkanShader::unbind() const {

	}

}
