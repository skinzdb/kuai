#pragma once

#include "Texture.h"

namespace kuai {
	class TextureArray
	{
	public:
		virtual ~TextureArray() = default;

		virtual void insert(std::shared_ptr<Texture> texture) = 0;
		virtual void remove(std::shared_ptr<Texture> texture) = 0;

		virtual void bind(uint32_t active_tex) = 0;

		virtual uint32_t get_id() const = 0;

		static std::shared_ptr<TextureArray> create(uint32_t width, uint32_t height, uint32_t layers);
	};
}