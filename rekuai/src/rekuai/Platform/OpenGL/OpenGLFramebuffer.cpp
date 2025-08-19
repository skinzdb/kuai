#include "rekuai/Platform/OpenGL/OpenGLFramebuffer.h"

#include "rekuai/Core/Core.h"
#include "glad/glad.h"

namespace kuai {
	OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height, uint32_t samples, uint32_t attachments)
	{
		props.width = width;
		props.height = height;
		props.samples = samples;
		props.attachments = attachments;

		reset();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &id);
		glDeleteTextures(col_attachments.size(), col_attachments.data());
		glDeleteTextures(1, &depth_attachments);
	}

	const uint32_t OpenGLFramebuffer::get_depth_attachment()
	{
		return depth_attachments;
	}

	const std::vector<uint32_t>& OpenGLFramebuffer::get_col_attachments()
	{
		return col_attachments;
	}

	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glBindTexture(get_texture_target(props.samples > 1), depth_attachments);
		// glViewport(0, 0, props.width, props.height);
	}

	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > MAX_FRAMEBUFFER_SIZE || height > MAX_FRAMEBUFFER_SIZE)
		{
			KU_CORE_WARN("Attempted to rezize framebuffer to {0}x{1}", width, height);
			return;
		}

		props.width = width;
		props.height = height;

		reset();
	}

	void OpenGLFramebuffer::reset()
	{
		if (id)
		{
			glDeleteFramebuffers(1, &id);
			glDeleteTextures(col_attachments.size(), col_attachments.data());
			glDeleteTextures(1, &depth_attachments);

			col_attachments.clear();
			depth_attachments = 0;
		}

		// Create framebuffer
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		bool multisampling = props.samples > 1;

		// Add colour attachments
		if (props.attachments)
		{
			col_attachments.resize(props.attachments);
			glGenTextures(props.attachments, col_attachments.data());

			for (size_t i = 0; i < col_attachments.size(); i++)
			{
				glBindTexture(get_texture_target(multisampling), col_attachments[i]);

				attach_col_texture(i);
			}
		}
		else
		{
			// Depth-only framebuffer so specify no drawing (or reading)
			glDrawBuffer(GL_NONE);
			// glReadBuffer(GL_NONE);
		}

		// Add depth attachment
		glGenTextures(1, &depth_attachments);
		glBindTexture(get_texture_target(multisampling), depth_attachments);
		attach_depth_texture();

		KU_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::attach_col_texture(uint32_t index)
	{
		bool multisampling = props.samples > 1;
		if (multisampling)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, props.samples, GL_RGB, props.width, props.height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, props.width, props.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, get_texture_target(multisampling), col_attachments[index], 0);
	}

	void OpenGLFramebuffer::attach_depth_texture()
	{
		bool multisampling = props.samples > 1;
		if (multisampling)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, props.samples, GL_DEPTH_ATTACHMENT, props.width, props.height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, props.width, props.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			// Add border so everything sampled outside of depth map range is set to 1.0
			float borderCol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderCol);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, get_texture_target(multisampling), depth_attachments, 0);
	}

	unsigned int OpenGLFramebuffer::get_texture_target(bool multisampling)
	{
		return multisampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}
}
