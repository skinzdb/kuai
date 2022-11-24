#pragma once

#include "smpch.h"

namespace Smong {
	class SMONGINE_API Shader
	{
	public:
		Shader(const char* vertSrc, const char* fragSrc);
		~Shader();

		void CreateUniform(std::string name);

		void SetUniform(std::string name, int val);
		void SetUniform(std::string name, float val);

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


