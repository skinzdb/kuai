#include "smpch.h"
#include "Shader.h"

#include <glad/glad.h>

Smong::Shader::~Shader()
{
	Unbind();
	if (programId)
		glDeleteProgram(programId);
}

std::shared_ptr<Smong::Shader> Smong::Shader::Create(const char* vertSrc, const char* fragSrc)
{
	vertShaderId = CreateShader(vertSrc, GL_VERTEX_SHADER);
	fragShaderId = CreateShader(fragSrc, GL_FRAGMENT_SHADER);

	return std::shared_ptr<Shader>();
}

void Smong::Shader::CreateUniform(std::string name)
{
	int uniformLoc = glGetUniformLocation(programId, name.c_str());
	if (uniformLoc < -1)
		SM_CORE_ERROR("Could not find the uniform {0}", name);
	uniforms[name] = uniformLoc;
}

void Smong::Shader::Bind()
{
	glUseProgram(programId);
}

void Smong::Shader::Unbind()
{
	glUseProgram(GL_NONE);
}

int Smong::Shader::CreateShader(const char* src, int type)
{
	int shaderId = glCreateShader(type);
	if (!shaderId)
		SM_CORE_ERROR("Failed to create shader ({0})", type);

	glShaderSource(shaderId, 1,  &src, nullptr);
	glCompileShader(shaderId);

	int compileSuccess;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccess);
	if (!compileSuccess)
	{
		int errLen; char* errStr;
		glGetShaderInfoLog(shaderId, 1024, &errLen, errStr); // Set max length of character buffer to 1024
		SM_CORE_ERROR("Error compiling shader code: {0}", errStr);
	}

	glAttachShader(programId, shaderId);

	return shaderId;
}

void Smong::Shader::Link()
{
	glLinkProgram(programId);
	int linkSuccess;
	glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess)
	{
		int errLen; char* errStr;
		glGetProgramInfoLog(programId, 1024, &errLen, errStr);
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
		int errLen; char* errStr;
		glGetProgramInfoLog(programId, 1024, &errLen, errStr);
		SM_CORE_ERROR("Error validating shader code: {0}", errStr);
	}
}
