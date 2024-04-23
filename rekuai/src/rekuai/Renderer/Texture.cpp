#include "kpch.h"
#include "Texture.h"
#include "TextureManager.h"

namespace kuai {
	u32 Texture::next_id = 0;

	Texture::Texture(const std::string& filename) : id(next_id++), filename(filename)
	{
		TextureManager::insert(*this);
	}

	Texture::~Texture()
	{
		TextureManager::remove(*this);
	}
}