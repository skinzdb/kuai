#pragma once

#include <glm/glm.hpp>

namespace kuai {

	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertSrc, const std::string& fragSrc);
		~ShaderProgram();

		void createUniform(const std::string& name);
		void setUniform(const std::string& name, int val) const;
		void setUniform(const std::string& name, float val) const;
		void setUniform(const std::string& name, const glm::vec2& val) const;
		void setUniform(const std::string& name, const glm::vec3& val) const;
		void setUniform(const std::string& name, const glm::vec4& val) const;
		void setUniform(const std::string& name, const glm::mat3& val) const;
		void setUniform(const std::string& name, const glm::mat4& val) const;

		void createUniformBlock(const std::string& name, const std::vector<const char*>& memberNames);
		void setUniform(const std::string& blockName, const std::string& memberName, const void* data, uint32_t size) const;

		void bind();
		void unbind();

	protected:
		int createShader(const char* src, int type);
		void link();

		int programId;
		int vertShaderId;
		int fragShaderId;

		std::unordered_map<std::string, uint32_t> uniforms;

		std::unordered_map<std::string, uint32_t> ubos;
		std::unordered_map<std::string, uint32_t> uboUniformOffsets;
	};
}


