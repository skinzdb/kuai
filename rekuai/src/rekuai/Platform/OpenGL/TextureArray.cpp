#include "kpch.h"

#include "TextureArray.h"

#include "glad/glad.h"
#include "stb_image_resize.h"

namespace kuai {

	//Texture::Texture()
	//{
	//	unsigned char data[] = { 0xFF, 0xFF, 0xFF };

	//	glGenTextures(1, &id);
	//	glBindTexture(GL_TEXTURE_2D, id);

	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//}

	TextureArray::TextureArray(uint32_t width, uint32_t height, uint32_t layers)
		: width(width), height(height), layers(layers)
	{
		//glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &capacity);

		//glGenTextures(1, &id);
		//glBindTexture(GL_TEXTURE_2D_ARRAY, id);

		//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, layers);

		//// Set texture wrapping options
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//// Set filtering options for down/upscaling
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	TextureArray::~TextureArray()
	{
		glDeleteTextures(1, &id);
	}

	void TextureArray::insert(const Texture& tex)
	{
//		KU_CORE_ASSERT(tex_ids.size() <= capacity, "Maximum number of textures exceeded");
//
//		if (std::find(tex_ids.begin(), tex_ids.end(), tex.id) != tex_ids.end())
//			return;
//
//		int pow2 = pow(2, floor(log2(tex.width)));
//
//
//
//		//stbir_resize_uint8();
//
//
//		tex_ids.push_back(tex.id);
//
////		glBindTexture(GL_TEXTURE_2D, )
//
//		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, tex.id, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	}

	void TextureArray::remove(const Texture& tex)
	{

	}
}
