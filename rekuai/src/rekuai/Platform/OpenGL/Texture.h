#pragma once

namespace kuai {
	/** \class Texture
	*	\brief A 2D texture that can support transparency.
	*/
	class Texture {
	public:
		/**
		* Loads texture from image file.
		*/
		Texture(const std::string& filename);
		~Texture();

	private:
		uint32_t id;
		std::string filename;

		static uint32_t next_id;

		friend class TextureManager;
	};
}
