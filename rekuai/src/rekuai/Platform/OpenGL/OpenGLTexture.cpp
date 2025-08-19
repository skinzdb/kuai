#include "kpch.h"
#include "Texture.h"
#include "TextureManager.h"

namespace kuai {
	uint32_t Texture::next_id = 0;

	Texture::Texture(const std::string& filename) : id(next_id++), filename(filename)
	{
		TextureManager::insert(*this);
	}

	Texture::~Texture()
	{
		TextureManager::remove(*this);
	}
}
