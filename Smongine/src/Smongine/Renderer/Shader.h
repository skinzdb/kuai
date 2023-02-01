#pragma once

#include "smpch.h"
#include "glm/mat4x4.hpp"

namespace Smong {
	class Shader
	{
	public:
		Shader(const char* vertSrc, const char* fragSrc);
		~Shader();

		void CreateUniform(const std::string& name);
		void CreateUniformBlock(const std::string& name, const std::vector<const char*>& memberNames, uint32_t size);

		void SetUniform(const std::string& name, int val) const;
		void SetUniform(const std::string& name, float val) const;
		void SetUniform(const std::string& name, const glm::vec2& val) const;
		void SetUniform(const std::string& name, const glm::vec3& val) const;
		void SetUniform(const std::string& name, const glm::vec4& val) const;
		void SetUniform(const std::string& name, const glm::mat3& val) const;
		void SetUniform(const std::string& name, const glm::mat4& val) const;
		
		void SetUniform(const std::string& blockName, const std::string& name, const glm::mat4& val) const;

		void Bind();
		void Unbind();

	private:
		int CreateShader(const char* src, int type);
		void Link();

		int programId;
		int vertShaderId;
		int fragShaderId;

		std::unordered_map<std::string, int> uniforms;
		std::unordered_map<std::string, int> uniformOffsets;
	};
}


