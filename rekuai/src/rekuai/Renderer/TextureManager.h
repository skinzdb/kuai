#pragma once

namespace kuai {
	class Texture;

	const u32 TEX_ARRAY_COUNT = 8;

	const u32 TEX_ARRAY_WIDTH = 256;
	const u32 TEX_ARRAY_HEIGHT = 256;

	class TextureManager
	{
	public:
		static void init();
		static void cleanup();

		static void insert(const Texture& tex);
		static void remove(const Texture& tex);
	
	private:
		static std::vector<u32> tex_ids;

		static unsigned int tex_array_ids[TEX_ARRAY_COUNT];
		static int capacity;
	};
}