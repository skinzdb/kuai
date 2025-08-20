#include "kpch.h"

#include "OpenGLTextureArray.h"

#include "glad/glad.h"
#include "stb_image_resize.h"

namespace kuai {

	OpenGLTextureArray::OpenGLTextureArray(uint32_t width, uint32_t height, uint32_t layers) 
		: width(width), height(height), layers(layers)
	{
		this->width = width;

		
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D_ARRAY, id);

		// Allocate the storage; only use 1 mip level
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, layers);

		// Set texture wrapping options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set filtering options for down/upscaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	OpenGLTextureArray::~OpenGLTextureArray()
	{
		// glDeleteTextures(GL_TEXTURE_2D_ARRAY, &id);
	}

	void OpenGLTextureArray::insert(std::shared_ptr<Texture> texture)
	{
		if (tex_map.count(texture->get_id()) != 0) {
			return;
		}

		tex_map[texture->get_id()]++;

		unsigned char* outData = new unsigned char[(size_t) width * height * 4];

		if (texture->get_width() > width || texture->get_height() > height)
		{
			unsigned char* inData = new unsigned char[(size_t)texture->get_width() * texture->get_height() * 4];

			glBindTexture(GL_TEXTURE_2D, texture->get_id());
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, inData);

			stbir_resize_uint8(inData, texture->get_width(), texture->get_height(), 0, outData, width, height, 0, 4);

			delete[] inData;
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture->get_id());
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, outData);
		}

		glBindTexture(GL_TEXTURE_2D_ARRAY, id);
		// First zero is mipmap level; next two zeros are x and y offsets; last zero is layer index offset
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texture->get_id(), width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, outData);

		delete[] outData;
	}

	void OpenGLTextureArray::remove(std::shared_ptr<Texture> texture)
	{
	}

	void OpenGLTextureArray::bind(uint32_t activeTex)
	{
		glActiveTexture(GL_TEXTURE0 + activeTex);
		glBindTexture(GL_TEXTURE_2D_ARRAY, id);
	}
}
