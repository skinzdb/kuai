#include "kpch.h"
#include "Texture.h"

#include "glad/glad.h"
#include "stb_image.h"

namespace kuai {
	Texture::Texture()
	{
		unsigned char data[] = { 0xFF, 0xFF, 0xFF };

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	Texture::Texture(const std::string& filename)
	{
		KU_PROFILE_FUNCTION();

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Load file using stbi library
		int width, height, colourChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &colourChannels, 0);
	
		if (data)
		{
			GLenum format = 0;
			if (colourChannels == 1)
				format = GL_RED;
			else if (colourChannels == 3)
				format = GL_RGB;
			else if (colourChannels == 4)
				format = GL_RGBA;

			KU_CORE_ASSERT(format, "Texture file format not supported");

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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

	Texture::~Texture()
	{
		glDeleteTextures(1, &textureId);
	}

	uint32_t Texture::getId()
	{
		return textureId;
	}

	void Texture::bind(uint32_t activeTex)
	{
		glActiveTexture(GL_TEXTURE0 + activeTex);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
}
