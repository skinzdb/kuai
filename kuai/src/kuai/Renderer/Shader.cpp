#include "kpch.h"
#include "Shader.h"

#include "kuai/Util/FileUtil.h" // REMOVE after creating shaders properly

#include <glad/glad.h>

namespace kuai {
	Shader::Shader(const char* vertSrc, const char* fragSrc)
	{
		programId = glCreateProgram();
		vertShaderId = createShader(vertSrc, GL_VERTEX_SHADER);
		fragShaderId = createShader(fragSrc, GL_FRAGMENT_SHADER);
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

	void Shader::createUniformBlock(const std::string& name, const std::vector<const char*>& memberNames, uint32_t size)
	{
		KU_CORE_ASSERT(memberNames.size(), "No member names entered for uniform block.");

		// Create uniform buffer object
		unsigned int ubo;
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, size);

		// Define the range of the buffer that links to a uniform binding point

		uniforms[name] = ubo;

		// Get indices of uniforms within block
		unsigned int* indices = new unsigned int[memberNames.size()];
		glGetUniformIndices(programId, memberNames.size(), memberNames.data(), indices);
		// Get offsets for uniforms in bytes
		int* offsets = new int[std::max(1, (int)memberNames.size())];
		glGetActiveUniformsiv(programId, memberNames.size(), indices, GL_UNIFORM_OFFSET, offsets);

		for (int i = 0; i < memberNames.size(); i++)
		{
			uniformOffsets[memberNames[i]] = offsets[i];
		}

		delete[] indices;
		delete[] offsets;
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

	void Shader::setUniform(const std::string& blockName, const std::string& memberName, const glm::mat4& val) const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uniforms.at(blockName));
		glBufferSubData(GL_UNIFORM_BUFFER, uniformOffsets.at(memberName), sizeof(glm::mat4), &val[0][0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
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

