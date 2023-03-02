#pragma once

#include "kpch.h"
#include "glm/mat4x4.hpp"

namespace kuai {
	class Shader
	{
	public:
		Shader(const char* vertSrc, const char* fragSrc);
		~Shader();

		void createUniform(const std::string& name);
		void createUniformBlock(const std::string& name, const std::vector<const char*>& memberNames, uint32_t size);

		void setUniform(const std::string& name, int val) const;
		void setUniform(const std::string& name, float val) const;
		void setUniform(const std::string& name, const glm::vec2& val) const;
		void setUniform(const std::string& name, const glm::vec3& val) const;
		void setUniform(const std::string& name, const glm::vec4& val) const;
		void setUniform(const std::string& name, const glm::mat3& val) const;
		void setUniform(const std::string& name, const glm::mat4& val) const;
		
		void setUniform(const std::string& blockName, const std::string& name, const glm::mat4& val) const;

		void bind();
		void unbind();

	private:
		int createShader(const char* src, int type);
		void link();

		int programId;
		int vertShaderId;
		int fragShaderId;

		std::unordered_map<std::string, int> uniforms;
		std::unordered_map<std::string, int> uniformOffsets;
	};
}


