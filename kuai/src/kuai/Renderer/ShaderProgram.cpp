#include "kpch.h"
#include "ShaderProgram.h"

#include <glad/glad.h>

namespace kuai {
	ShaderProgram::ShaderProgram(const std::string& vertSrc, const std::string& fragSrc)
	{
		programId = glCreateProgram();
		vertShaderId = createShader(vertSrc.c_str(), GL_VERTEX_SHADER);
		fragShaderId = createShader(fragSrc.c_str(), GL_FRAGMENT_SHADER);
		link();
	}

	ShaderProgram::~ShaderProgram()
	{
		unbind();
		
		for (auto& pair : ubos)
			glDeleteBuffers(1, &pair.second);
		if (programId)
			glDeleteProgram(programId);
	}

	void ShaderProgram::createUniform(const std::string& name)
	{
		int uniformLoc = glGetUniformLocation(programId, name.c_str());
		if (uniformLoc == -1)
			KU_CORE_ERROR("[Shader {0}] Could not find the uniform {1}", programId, name);
		uniforms[name] = uniformLoc;
	}

	void ShaderProgram::setUniform(const std::string& name, int val) const
	{
		glUniform1i(uniforms.at(name), val);
	}

	void ShaderProgram::setUniform(const std::string& name, float val) const
	{
		glUniform1f(uniforms.at(name), val);
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::vec2& val) const
	{
		glUniform2f(uniforms.at(name), val.x, val.y);
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::vec3& val) const
	{
		glUniform3f(uniforms.at(name), val.x, val.y, val.z);
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::vec4& val) const
	{
		glUniform4f(uniforms.at(name), val.x, val.y, val.z, val.w);
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::mat3& val) const
	{
		glUniformMatrix3fv(uniforms.at(name), 1, GL_FALSE, &val[0][0]);
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::mat4& val) const
	{
		glUniformMatrix4fv(uniforms.at(name), 1, GL_FALSE, &val[0][0]);
	}

	void ShaderProgram::createUniformBlock(const std::string& name, const std::vector<const char*>& memberNames)
	{
		KU_CORE_ASSERT(memberNames.size(), "No member names entered for uniform block.");

		unsigned int blockIndex = glGetUniformBlockIndex(programId, name.c_str());
		int blockSize;
		glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		unsigned int* indices = new unsigned int[memberNames.size()];
		glGetUniformIndices(programId, memberNames.size(), memberNames.data(), indices);

		int* offsets = new int[std::max(1, (int)memberNames.size())]; // std::max to avoid some overflow warning
		glGetActiveUniformsiv(programId, memberNames.size(), indices, GL_UNIFORM_OFFSET, offsets);

		KU_CORE_INFO("{0}, binding={1}", name, blockIndex);
		for (int i = 0; i < memberNames.size(); i++)
		{
			KU_CORE_TRACE("{0}, offset={1}", memberNames[i], offsets[i]);
			uboUniformOffsets[memberNames[i]] = offsets[i];
		}

		delete[] indices;
		delete[] offsets;

		// Create uniform buffer object
		unsigned int ubo;
		glGenBuffers(1, &ubo);
		glNamedBufferData(ubo, blockSize, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, ubo);

		ubos[name] = ubo;
	}

	void ShaderProgram::setUniform(const std::string& blockName, const std::string& memberName, const void* data, uint32_t size) const
	{
		glNamedBufferSubData(ubos.at(blockName), uboUniformOffsets.at(memberName), size, data);
	}

	void ShaderProgram::bind()
	{
		glUseProgram(programId);
	}

	void ShaderProgram::unbind()
	{
		glUseProgram(GL_NONE);
	}

	int ShaderProgram::createShader(const char* src, int type)
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

	void ShaderProgram::link()
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