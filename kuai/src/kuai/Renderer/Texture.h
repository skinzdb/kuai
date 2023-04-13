#pragma once

namespace kuai {
	/** \class Texture
	*	\brief A 2D texture that can support transparency.
	*/
	class Texture {
	public:
		/**
		* Creates default blank texture.
		*/
		Texture();
		/**
		* Loads texture from image file.
		*/
		Texture(const std::string& filename);
		~Texture();
		/// @private
		uint32_t getId();
		/// @private
		void bind(uint32_t activeTex);
	private:
		uint32_t textureId;
	};
}