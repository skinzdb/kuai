#pragma once

#include <glm/glm.hpp>

#include "Buffer.h"

namespace kuai {

	class Shader
	{
	public:
		Shader(const std::string& vert_src, const std::string& frag_src);
		~Shader();

		void create_uniform(const std::string& name);
		void set_uniform(const std::string& name, int val) const;
		void set_uniform(const std::string& name, float val) const;
		void set_uniform(const std::string& name, const glm::vec2& val) const;
		void set_uniform(const std::string& name, const glm::vec3& val) const;
		void set_uniform(const std::string& name, const glm::vec4& val) const;
		void set_uniform(const std::string& name, const glm::mat3& val) const;
		void set_uniform(const std::string& name, const glm::mat4& val) const;

		void create_uniform_block(const std::string& name, const std::vector<const char*>& members, u32 binding);
		void set_uniform(const std::string& name, const std::string& member, const void* data, u32 size) const;

		void bind() const;
		void unbind() const;

	private:
		int create_shader(const char* src, int type);
		void link();

	private:
		int id;
		int vert_id;
		int frag_id;

		std::unordered_map<std::string, u32> uniforms;

		// UBOs and member offsets are static as they are shared between all shaders
		static std::unordered_map<std::string, u32> ubos;
		static std::unordered_map<std::string, u32> ubo_offsets;

		friend class Renderer;
	};
}


