#pragma once

#include <glm/glm.hpp>

namespace kuai {

	class Shader
	{
	public:
		Shader(const std::string& vertSrc, const std::string& fragSrc);
		~Shader();

		void createUniform(const std::string& name);
		void setUniform(const std::string& name, int val) const;
		void setUniform(const std::string& name, float val) const;
		void setUniform(const std::string& name, const glm::vec2& val) const;
		void setUniform(const std::string& name, const glm::vec3& val) const;
		void setUniform(const std::string& name, const glm::vec4& val) const;
		void setUniform(const std::string& name, const glm::mat3& val) const;
		void setUniform(const std::string& name, const glm::mat4& val) const;

		void createUniformBlock(const std::string& name, const std::vector<const char*>& members, uint32_t binding);
		void setUniform(const std::string& name, const std::string& member, const void* data, uint32_t size) const;

		void bind();
		void unbind();

	protected:
		int createShader(const char* src, int type);
		void link();

		int programId;
		int vertShaderId;
		int fragShaderId;

		std::unordered_map<std::string, uint32_t> uniforms;
		// Static maps of UBOs and member offsets as they are shared between all shaders
		static std::unordered_map<std::string, uint32_t> ubos;
		static std::unordered_map<std::string, uint32_t> uboOffsets;

		friend class StaticShader;
	};
}


