#pragma once

#include "smpch.h"

namespace Smong {
	class Texture {
	public:
		Texture();
		Texture(const char* filename);
		~Texture();

		uint32_t GetId();

		void Bind(uint32_t activeTex);
	private:
		uint32_t textureId;
	};
}