#include "Shader.h"

#include <glad/glad.h>

namespace kuai {
	std::unordered_map<std::string, uint32_t> Shader::ubos = std::unordered_map<std::string, uint32_t>();
	std::unordered_map<std::string, uint32_t> Shader::ubo_offsets = std::unordered_map<std::string, uint32_t>();

	Shader::Shader(const std::string& vert_src, const std::string& frag_src)
	{
		id = glCreateProgram();
		vert_id = create_shader(vert_src.c_str(), GL_VERTEX_SHADER);
		frag_id = create_shader(frag_src.c_str(), GL_FRAGMENT_SHADER);
		link();
	}

	Shader::~Shader()
	{
		unbind();

		for (auto& pair : ubos)
			glDeleteBuffers(1, &pair.second);

		if (id)
			glDeleteProgram(id);
	}

	void Shader::create_uniform(const std::string& name)
	{
		int uniformLoc = glGetUniformLocation(id, name.c_str());
		uniforms[name] = uniformLoc;
	}

	void Shader::set_uniform(const std::string& name, int val) const
	{
		glUniform1i(uniforms.at(name), val);
	}

	void Shader::set_uniform(const std::string& name, float val) const
	{
		glUniform1f(uniforms.at(name), val);
	}

	void Shader::set_uniform(const std::string& name, const glm::vec2& val) const
	{
		glUniform2f(uniforms.at(name), val.x, val.y);
	}

	void Shader::set_uniform(const std::string& name, const glm::vec3& val) const
	{
		glUniform3f(uniforms.at(name), val.x, val.y, val.z);
	}

	void Shader::set_uniform(const std::string& name, const glm::vec4& val) const
	{
		glUniform4f(uniforms.at(name), val.x, val.y, val.z, val.w);
	}

	void Shader::set_uniform(const std::string& name, const glm::mat3& val) const
	{
		glUniformMatrix3fv(uniforms.at(name), 1, GL_FALSE, &val[0][0]);
	}

	void Shader::set_uniform(const std::string& name, const glm::mat4& val) const
	{
		glUniformMatrix4fv(uniforms.at(name), 1, GL_FALSE, &val[0][0]);
	}

	void Shader::create_uniform_block(const std::string& name, const std::vector<const char*>& members, u32 binding)
	{
		// Get block index and block size
		uint32_t block_idx = glGetUniformBlockIndex(id, name.c_str());
		int block_size;
		glGetActiveUniformBlockiv(id, block_idx, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);

		// Get indices of member variables, and then their offsets
		GLuint* indices = new GLuint[members.size()];
		glGetUniformIndices(id, members.size(), &members[0], indices);
		GLint* offsets = new GLint[members.size()];
		glGetActiveUniformsiv(id, members.size(), indices, GL_UNIFORM_OFFSET, offsets);

		for (size_t i = 0; i < members.size(); i++)
		{
			ubo_offsets[members[i]] = offsets[i];
		}

		delete[] indices;
		delete[] offsets;

		// Create uniform buffer object
		uint32_t ubo;
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, block_size, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);

		ubos[name] = ubo;
	}

	void Shader::set_uniform(const std::string& name, const std::string& member, const void* data, u32 size)
	{
		glNamedBufferSubData(ubos.at(name), ubo_offsets.at(member), size, data);
	}

	void Shader::bind() const
	{
		glUseProgram(id);
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
	}

	int Shader::create_shader(const char* src, int type)
	{
		int shaderId = glCreateShader(type);
		if (!shaderId)
		{
			KU_CORE_ERROR("[Shader {0}] Failed to create shader ({1})", id, type);
		}

		glShaderSource(shaderId, 1, &src, nullptr);
		glCompileShader(shaderId);

		int compileSuccess;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccess);
		if (!compileSuccess)
		{
			char errStr[1024];
			glGetShaderInfoLog(shaderId, 1024, nullptr, errStr); // Set max length of character buffer to 1024
			KU_CORE_ERROR("[Shader {0}] Error compiling shader code: {1}", id, errStr);
		}

		glAttachShader(id, shaderId);

		return shaderId;
	}

	void Shader::link()
	{
		glLinkProgram(id);
		int linkSuccess;
		glGetProgramiv(id, GL_LINK_STATUS, &linkSuccess);
		if (!linkSuccess)
		{
			char errStr[1024];
			glGetProgramInfoLog(id, 1024, nullptr, errStr);
			KU_CORE_ERROR("[Shader {0}] Error linking shader code: {1}", id, errStr);
		}

		if (vert_id)
		{
			glDetachShader(id, vert_id);
		}
		if (frag_id)
		{
			glDetachShader(id, frag_id);
		}

		glValidateProgram(id);
		int validateSuccess;
		glGetProgramiv(id, GL_VALIDATE_STATUS, &validateSuccess);
		if (!validateSuccess)
		{
			char errStr[1024];
			glGetProgramInfoLog(id, 1024, nullptr, errStr);
			KU_CORE_ERROR("[Shader {0}] Error validating shader code: {1}", id, errStr);
		}
	}
}