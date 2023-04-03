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

		uint32_t createUniformBlock(const std::string& name);
		void setUniform(uint32_t bufId, const void* data, uint32_t size, uint32_t offset = 0) const;

		void deleteBuffer(uint32_t bufId);

		void bind();
		void unbind();

	protected:
		int createShader(const char* src, int type);
		void link();

		int programId;
		int vertShaderId;
		int fragShaderId;

		std::unordered_map<std::string, uint32_t> uniforms;
		//std::unordered_map<std::string, uint32_t> ubos;

		friend class StaticShader;
	};
}


