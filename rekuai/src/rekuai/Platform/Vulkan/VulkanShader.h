#pragma once

#include "rekuai/Renderer/Shader.h"

namespace kuai {
    class VulkanShader : public Shader {

    public:
       	VulkanShader(const std::string& vert_src, const std::string& frag_src);
		virtual ~VulkanShader();

		virtual void create_uniform(const std::string& name) override;
		virtual void set_uniform(const std::string& name, int val) const override;
		virtual void set_uniform(const std::string& name, float val) const override;
		virtual void set_uniform(const std::string& name, const glm::vec2& val) const override;
		virtual void set_uniform(const std::string& name, const glm::vec3& val) const override;
		virtual void set_uniform(const std::string& name, const glm::vec4& val) const override;
		virtual void set_uniform(const std::string& name, const glm::mat3& val) const override;
		virtual void set_uniform(const std::string& name, const glm::mat4& val) const override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t get_id() const override { return shader_id; }

	private:
	    uint32_t shader_id;
    };
}
