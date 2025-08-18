#pragma once

#include <glm/glm.hpp>
#include <memory>

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

		static std::unique_ptr<Shader> create(const std::string& vert_src, const std::string& frag_src);

	private:
		int create_shader(const char* src, int type);
		void link();

	private:
		int id;
		int vert_id;
		int frag_id;

		std::unordered_map<std::string, uint32_t> uniforms;

		// UBOs and member offsets are static as they are shared between all shaders
		static std::unordered_map<std::string, uint32_t> ubos;
		static std::unordered_map<std::string, uint32_t> ubo_offsets;

		friend class Renderer;
	};
}
