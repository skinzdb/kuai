#pragma once

#include "smpch.h"

#include "glm/mat4x4.hpp"

namespace Smong {
	class SMONGINE_API Shader
	{
	public:
		Shader(const char* vertSrc, const char* fragSrc);
		~Shader();

		void CreateUniform(std::string name);

		void SetUniform(std::string name, int val);
		void SetUniform(std::string name, float val);
		void SetUniform(std::string name, glm::vec2& val);
		void SetUniform(std::string name, glm::vec3& val);
		void SetUniform(std::string name, glm::vec4& val);
		void SetUniform(std::string name, glm::mat4& val);
		
		void Bind();
		void Unbind();
	private:
		int CreateShader(const char* src, int type);
		void Link();

		int programId;
		int vertShaderId;
		int fragShaderId;
		std::unordered_map<std::string, int> uniforms;
	};
}


