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
		u32 id;
		std::string filename;

		static u32 next_id;

		friend class TextureManager;
	};
}