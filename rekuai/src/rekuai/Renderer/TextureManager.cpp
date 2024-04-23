#include "kpch.h"

#include "TextureManager.h"
#include "Texture.h"

#include "glad/glad.h"
#include "stb_image.h"
#include "stb_image_resize.h"

namespace kuai {
	void TextureManager::init()
	{
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &capacity);

		glGenTextures(TEX_ARRAY_COUNT, &tex_array_ids[0]);
		glBindTextures(GL_TEXTURE_2D_ARRAY, TEX_ARRAY_COUNT, &tex_array_ids[0]);

		glTexStorage3D(
			GL_TEXTURE_2D_ARRAY,	
			1, // No mipmaps
			GL_RGBA8, 
			TEX_ARRAY_WIDTH, 
			TEX_ARRAY_HEIGHT, 
			1  // Number of layers
		);

		// Set texture wrapping options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set filtering options for down/upscaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void TextureManager::cleanup()
	{
		glDeleteTextures(TEX_ARRAY_COUNT, &tex_array_ids[0]);
	}

	void TextureManager::insert(const Texture& tex)
	{
		// Load file using stbi library
		int width, height, col_channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(tex.filename.c_str(), &width, &height, &col_channels, 0);

		if (!data)
		{
			KU_CORE_ERROR("Failed to load texture: {0}", tex.filename);
			return;
		}

		GLenum format = 0;
		if (col_channels == 1)
			format = GL_RED;
		else if (col_channels == 3)
			format = GL_RGB;
		else if (col_channels == 4)
			format = GL_RGBA;

		KU_CORE_ASSERT(format, "Texture file format not supported");

		KU_CORE_ASSERT(tex_ids.size() <= capacity, "Maximum number of textures exceeded");

		if (std::find(tex_ids.begin(), tex_ids.end(), tex.id) != tex_ids.end())
			return;

		KU_CORE_INFO("Loaded texture: {0} ({1}x{2})", tex.filename, width, height);

		//unsigned char new_data[TEX_ARRAY_WIDTH * TEX_ARRAY_HEIGHT];

		//if (width > TEX_ARRAY_WIDTH || height > TEX_ARRAY_HEIGHT)
		//{
		//	stbir_resize_uint8(new_data, width, height, 0, data, TEX_ARRAY_WIDTH, TEX_ARRAY_HEIGHT, 0, 4);
		//}

		//int pow2 = pow(2, floor(log2(width)));


		tex_ids.push_back(tex.id);

		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, tex.id, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		stbi_image_free(data);
	}

	void TextureManager::remove(const Texture& tex)
	{
	}
}