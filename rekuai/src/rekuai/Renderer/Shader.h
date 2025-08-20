#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

namespace kuai {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void create_uniform(const std::string& name) = 0;
		virtual void set_uniform(const std::string& name, int val) const = 0;
		virtual void set_uniform(const std::string& name, float val) const = 0;
		virtual void set_uniform(const std::string& name, const glm::vec2& val) const = 0;
		virtual void set_uniform(const std::string& name, const glm::vec3& val) const = 0;
		virtual void set_uniform(const std::string& name, const glm::vec4& val) const = 0;
		virtual void set_uniform(const std::string& name, const glm::mat3& val) const = 0;
		virtual void set_uniform(const std::string& name, const glm::mat4& val) const = 0;

		virtual void create_uniform_block(const std::string& name, const std::vector<const char*>& members, uint32_t binding) = 0;
		virtual void set_uniform_block(const std::string& name, const std::string& member, const void* data, uint32_t size) = 0;

		virtual uint32_t get_id() const = 0;

	public:
		static std::shared_ptr<Shader> create(const std::string& vert_src, const std::string& frag_src);
		static std::shared_ptr<Shader> get(uint32_t prog_id);

	protected:
		// UBOs and member offsets are static as they are shared between all shaders
		static std::unordered_map<std::string, uint32_t> ubos;
		static std::unordered_map<std::string, uint32_t> ubo_offsets;

	private:
		static std::unordered_map<uint32_t, std::shared_ptr<Shader>> shader_map;
	};
}
