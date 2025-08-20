#pragma once

#include <glm/glm.hpp>

#include "rekuai/Renderer/Shader.h"

namespace kuai {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vert_src, const std::string& frag_src);
		virtual ~OpenGLShader();

		virtual void create_uniform(const std::string& name) override;
		virtual void set_uniform(const std::string& name, int val) const override;
		virtual void set_uniform(const std::string& name, float val) const override;
		virtual void set_uniform(const std::string& name, const glm::vec2& val) const override;
		virtual void set_uniform(const std::string& name, const glm::vec3& val) const override;
		virtual void set_uniform(const std::string& name, const glm::vec4& val) const override;
		virtual void set_uniform(const std::string& name, const glm::mat3& val) const override;
		virtual void set_uniform(const std::string& name, const glm::mat4& val) const override;

		// void create_uniform_block(const std::string& name, const std::vector<const char*>& members, uint32_t binding);
		// static void set_uniform(const std::string& name, const std::string& member, const void* data, uint32_t size);

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t get_id() const override { return id; }

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
	};
}
