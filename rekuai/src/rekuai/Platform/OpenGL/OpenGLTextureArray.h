#pragma once

#include "rekuai/Renderer/TextureArray.h"

namespace kuai {
	class OpenGLTextureArray : public TextureArray
	{
	public:
		OpenGLTextureArray(uint32_t width, uint32_t height, uint32_t layers);
		virtual ~OpenGLTextureArray() override;

		virtual void insert(std::shared_ptr<Texture> texture) override;
		virtual void remove(std::shared_ptr<Texture> texture) override;

		virtual uint32_t get_id() const override { return id; }

		void bind(uint32_t active_tex) override;
		
	private:
		uint32_t id;

		uint32_t width, height;
		uint32_t layers;

		std::unordered_map<uint32_t, uint32_t> tex_map;
	};
}