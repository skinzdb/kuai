#pragma once

namespace kuai {
	class Texture {
	public:
		Texture();
		Texture(const char* filename);
		~Texture();

		uint32_t getId();

		void bind(uint32_t activeTex);
	private:
		uint32_t textureId;
	};
}