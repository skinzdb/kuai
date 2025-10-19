#pragma once

#include "rekuai/Renderer/Shader.h"

#include <vulkan/vulkan.h>

namespace kuai {
    class VulkanShader : public Shader
    {
    public:
       	VulkanShader(VkDevice device, const std::string& vert_src, const std::string& frag_src);
		virtual ~VulkanShader();

		virtual void create_uniform(const std::string& name) override;
		virtual void set_uniform(const std::string& name, int val) const override;
		virtual void set_uniform(const std::string& name, float val) const override;
		virtual void set_uniform(const std::string& name, const glm::vec2& val) const override;
		virtual void set_uniform(const std::string& name, const glm::vec3& val) const override;
		virtual void set_uniform(const std::string& name, const glm::vec4& val) const override;
		virtual void set_uniform(const std::string& name, const glm::mat3& val) const override;
		virtual void set_uniform(const std::string& name, const glm::mat4& val) const override;

		virtual void create_uniform_block(const std::string& name, const std::vector<const char*>& members, uint32_t binding) override;
		virtual void set_uniform_block(const std::string& name, const std::string& member, const void* data, uint32_t size) override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t get_id() const override { return 0; }

	private:
		VkShaderEXT shaders[2]; // vertex: shaders[0], fragment: shaders[1] 

		VkDevice device;

        PFN_vkCmdBindShadersEXT bind_shaders_fn;

		friend class VulkanAPI;
		friend class VulkanCommand;
    };
}
