#include "VulkanShader.h"

namespace kuai {
    VulkanShader::VulkanShader(const std::string& vert_src, const std::string& frag_src) {

    }

    VulkanShader::~VulkanShader() {

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
