#include "kpch.h"
#include "Framebuffer.h"

#include "glad/glad.h"

namespace kuai {
	Framebuffer::Framebuffer(uint32_t width, uint32_t height, uint32_t samples, uint32_t attachments)
	{
		props.width = width;
		props.height = height;
		props.samples = samples;
		props.attachments = attachments;

		reset();
	}

	Framebuffer::Framebuffer() // Default framebuffer, don't do anything
	{
		props.width = 1280;
		props.height = 720;
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &frameBufId);
		glDeleteTextures(colAttachments.size(), colAttachments.data());
		glDeleteTextures(1, &depthAttachment);
	}

	void Framebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufId);
		glViewport(0, 0, props.width, props.height);
	}

	void Framebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::resize(uint32_t width, uint32_t height)
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

	void Framebuffer::reset()
	{
		if (frameBufId)
		{
			glDeleteFramebuffers(1, &frameBufId);
			glDeleteTextures(colAttachments.size(), colAttachments.data());
			glDeleteTextures(1, &depthAttachment);

			colAttachments.clear();
			depthAttachment = 0;
		}

		// Create framebuffer
		glGenFramebuffers(1, &frameBufId);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufId);

		bool multisampling = props.samples > 1;

		// Add colour attachments
		if (props.attachments)
		{
			colAttachments.resize(props.attachments);
			glGenTextures(props.attachments, colAttachments.data());

			for (size_t i = 0; i < colAttachments.size(); i++)
			{
				glBindTexture(getTextureTarget(multisampling), colAttachments[i]);

				attachColTexture(i);
			}
		}
		else
		{
			// Depth-only framebuffer so specify no drawing (or reading)
			glReadBuffer(GL_NONE);
			glDrawBuffer(GL_NONE);
		}

		// Add depth attachment
		glGenTextures(1, &depthAttachment);
		glBindTexture(getTextureTarget(multisampling), depthAttachment);
		attachDepthTexture();

		KU_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::attachColTexture(uint32_t index)
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

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, getTextureTarget(multisampling), colAttachments[index], 0);
	}

	void Framebuffer::attachDepthTexture()
	{
		bool multisampling = props.samples > 1;
		if (multisampling)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, props.samples, GL_DEPTH24_STENCIL8, props.width, props.height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, props.width, props.height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, getTextureTarget(multisampling), depthAttachment, 0);
	}

	unsigned int Framebuffer::getTextureTarget(bool multisampling)
	{
		return multisampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}
}