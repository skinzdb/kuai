#include "smpch.h"
#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Smong {
	Texture::Texture(const char* filename)
	{
		glGenTextures(1, &textureId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Set texture wrapping options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set filtering options for down/upscaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load file using stbi library
		int width, height, colourChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename, &width, &height, &colourChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			SM_CORE_ERROR("Failed to load texture: {0}", filename);
		}

		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &textureId);
	}

	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
}
