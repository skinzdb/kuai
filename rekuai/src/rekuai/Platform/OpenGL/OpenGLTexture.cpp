#include "kpch.h"

#include "OpenGLTexture.h"
#include "rekuai/Core/Core.h"

#include "glad/glad.h"
#include "stb_image.h"

namespace kuai {
	OpenGLTexture::OpenGLTexture() : width(1), height(1), format(TextureFormat::RGB)
	{
		unsigned char data[] = { 0xFF, 0xFF, 0xFF };
		
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture::OpenGLTexture(const std::string& filename)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		// Load file using stbi library
		int width, height, colourChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &colourChannels, 0);
		this->width = width;
		this->height = height;
		if (data)
		{
			GLenum format = 0;
			if (colourChannels == 1)
			{
				format = GL_RED;
			}
			else if (colourChannels == 3)
			{
				format = GL_RGB;
				this->format = TextureFormat::RGB;
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			}
			else if (colourChannels == 4)
			{
				format = GL_RGBA;
				this->format = TextureFormat::RGBA;
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			}

			KU_CORE_ASSERT(format, "Texture file format not supported");

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			// Set texture wrapping options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// Set filtering options for down/upscaling
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			KU_CORE_INFO("Loaded texture: {0} ({1}x{2})", filename, width, height);
		}
		else
		{
			KU_CORE_ERROR("Failed to load texture: {0}", filename);
		}

		stbi_image_free(data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &id);
	}

	void OpenGLTexture::bind(uint32_t active_tex) const
	{
		glActiveTexture(GL_TEXTURE0 + active_tex);
		glBindTexture(GL_TEXTURE_2D, id);
	}
}