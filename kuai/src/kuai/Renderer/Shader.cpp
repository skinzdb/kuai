#include "kpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace kuai {
	Shader::Shader(const std::string& vertSrc, const std::string& fragSrc)
	{
		programId = glCreateProgram();
		vertShaderId = createShader(vertSrc.c_str(), GL_VERTEX_SHADER);
		fragShaderId = createShader(fragSrc.c_str(), GL_FRAGMENT_SHADER);
		link();
	}

	Shader::~Shader()
	{
		unbind();
		
		if (programId)
			glDeleteProgram(programId);
	}

	void Shader::createUniform(const std::string& name)
	{
		int uniformLoc = glGetUniformLocation(programId, name.c_str());
		if (uniformLoc == -1)
			KU_CORE_ERROR("[Shader {0}] Could not find the uniform {1}", programId, name);
		uniforms[name] = uniformLoc;
	}

	void Shader::setUniform(const std::string& name, int val) const
	{
		glUniform1i(uniforms.at(name), val);
	}

	void Shader::setUniform(const std::string& name, float val) const
	{
		glUniform1f(uniforms.at(name), val);
	}

	void Shader::setUniform(const std::string& name, const glm::vec2& val) const
	{
		glUniform2f(uniforms.at(name), val.x, val.y);
	}

	void Shader::setUniform(const std::string& name, const glm::vec3& val) const
	{
		glUniform3f(uniforms.at(name), val.x, val.y, val.z);
	}

	void Shader::setUniform(const std::string& name, const glm::vec4& val) const
	{
		glUniform4f(uniforms.at(name), val.x, val.y, val.z, val.w);
	}

	void Shader::setUniform(const std::string& name, const glm::mat3& val) const
	{
		glUniformMatrix3fv(uniforms.at(name), 1, GL_FALSE, &val[0][0]);
	}

	void Shader::setUniform(const std::string& name, const glm::mat4& val) const
	{
		glUniformMatrix4fv(uniforms.at(name), 1, GL_FALSE, &val[0][0]);
	}

	uint32_t Shader::createUniformBlock(const std::string& name)
	{
		// Get binding index and block size
		uint32_t blockIndex = glGetUniformBlockIndex(programId, name.c_str());
		int blockSize;
		glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		// Create uniform buffer object
		uint32_t ubo;
		glGenBuffers(1, &ubo);
		glNamedBufferData(ubo, blockSize, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

		KU_CORE_WARN(ubo);
		
		return ubo;
	}

	void Shader::setUniform(uint32_t bufId, const void* data, uint32_t size, uint32_t offset) const
	{
		glNamedBufferSubData(bufId, offset, size, data);
	}

	void Shader::deleteBuffer(uint32_t bufId)
	{
		glDeleteBuffers(1, &bufId);
	}

	void Shader::bind()
	{
		glUseProgram(programId);
	}

	void Shader::unbind()
	{
		glUseProgram(GL_NONE);
	}

	int Shader::createShader(const char* src, int type)
	{
		int shaderId = glCreateShader(type);
		if (!shaderId)
			KU_CORE_ERROR("[Shader {0}] Failed to create shader ({1})", programId, type);

		glShaderSource(shaderId, 1, &src, nullptr);
		glCompileShader(shaderId);

		int compileSuccess;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccess);
		if (!compileSuccess)
		{
			char errStr[1024];
			glGetShaderInfoLog(shaderId, 1024, nullptr, errStr); // Set max length of character buffer to 1024
			KU_CORE_ERROR("[Shader {0}] Error compiling shader code: {1}", programId, errStr);
		}

		glAttachShader(programId, shaderId);

		return shaderId;
	}

	void Shader::link()
	{
		glLinkProgram(programId);
		int linkSuccess;
		glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccess);
		if (!linkSuccess)
		{
			char errStr[1024];
			glGetProgramInfoLog(programId, 1024, nullptr, errStr);
			KU_CORE_ERROR("[Shader {0}] Error linking shader code: {1}", programId, errStr);
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
			KU_CORE_ERROR("[Shader {0}] Error validating shader code: {1}", programId, errStr);
		}
	}
}