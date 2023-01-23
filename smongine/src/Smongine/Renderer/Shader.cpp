#include "smpch.h"
#include "Shader.h"

#include "Smongine/Util/FileUtil.h" // REMOVE after creating shaders properly

#include <glad/glad.h>

namespace Smong {
	Shader::Shader(const char* vertSrc, const char* fragSrc)
	{
		programId = glCreateProgram();
		vertShaderId = CreateShader(vertSrc, GL_VERTEX_SHADER);
		fragShaderId = CreateShader(fragSrc, GL_FRAGMENT_SHADER);
		Link();
	}

	Shader::~Shader()
	{
		Unbind();
		if (programId)
			glDeleteProgram(programId);
	}

	void Shader::CreateUniform(std::string name)
	{
		int uniformLoc = glGetUniformLocation(programId, name.c_str());
		if (uniformLoc < -1)
			SM_CORE_ERROR("Could not find the uniform {0}", name);
		uniforms[name] = uniformLoc;
	}

	void Shader::SetUniform(std::string name, int val)
	{
		glUniform1i(uniforms[name], val);
	}

	void Shader::SetUniform(std::string name, float val)
	{
		glUniform1f(uniforms[name], val);
	}

	void Shader::SetUniform(std::string name, glm::vec2& val)
	{
		glUniform2f(uniforms[name], val.x, val.y);
	}

	void Shader::SetUniform(std::string name, glm::vec3& val)
	{
		glUniform3f(uniforms[name], val.x, val.y, val.z);
	}

	void Shader::SetUniform(std::string name, glm::vec4& val)
	{
		glUniform4f(uniforms[name], val.x, val.y, val.z, val.w);
	}

	void Shader::SetUniform(std::string name, glm::mat3& val)
	{
		glUniformMatrix3fv(uniforms[name], 1, GL_FALSE, &val[0][0]);
	}

	void Shader::SetUniform(std::string name, glm::mat4& val)
	{
		glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, &val[0][0]);
	}

	void Shader::Bind()
	{
		glUseProgram(programId);
	}

	void Shader::Unbind()
	{
		glUseProgram(GL_NONE);
	}

	int Shader::CreateShader(const char* src, int type)
	{
		int shaderId = glCreateShader(type);
		if (!shaderId)
			SM_CORE_ERROR("Failed to create shader ({0})", type);

		glShaderSource(shaderId, 1, &src, nullptr);
		glCompileShader(shaderId);

		int compileSuccess;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccess);
		if (!compileSuccess)
		{
			char errStr[1024];
			glGetShaderInfoLog(shaderId, 1024, nullptr, errStr); // Set max length of character buffer to 1024
			SM_CORE_ERROR("Error compiling shader code: {0}", errStr);
		}

		glAttachShader(programId, shaderId);

		return shaderId;
	}

	void Shader::Link()
	{
		glLinkProgram(programId);
		int linkSuccess;
		glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccess);
		if (!linkSuccess)
		{
			char errStr[1024];
			glGetProgramInfoLog(programId, 1024, nullptr, errStr);
			SM_CORE_ERROR("Error linking shader code: {0}", errStr);
		}

		if (vertShaderId)
			glDetachShader(programId, vertShaderId);
		if (fragShaderId)
			glDetachShader(programId, fragShaderId);

		glValidateProgram(programId);
		int validateSuccess;
		glGetProgramiv(programId, GL_VALIDATE_STATUS, &validateSuccess);
		if (!validateSuccess)
		{
			char errStr[1024];
			glGetProgramInfoLog(programId, 1024, nullptr, errStr);
			SM_CORE_ERROR("Error validating shader code: {0}", errStr);
		}
	}
}

