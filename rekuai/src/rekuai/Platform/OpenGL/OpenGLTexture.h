#pragma once

#include "rekuai/Renderer/Texture.h"
#include <string>

namespace kuai {
	/** \class Texture
	*	\brief A 2D texture that can support transparency.
	*/
	class OpenGLTexture : public Texture 
	{
	public:
		/**
		* Creates default blank texture.
		*/
		OpenGLTexture();
		/**
		* Loads texture from image file.
		*/
		OpenGLTexture(const std::string& filename);
		virtual ~OpenGLTexture() override;

		virtual uint32_t get_width() const override { return width; }
		virtual uint32_t get_height() const override { return height; }
		
		virtual void bind(uint32_t active_tex) const override;
		
		virtual uint32_t get_id() const override { return id; }

	private:
		uint32_t id;

		uint32_t width;
		uint32_t height;

		TextureFormat format;
	};
}