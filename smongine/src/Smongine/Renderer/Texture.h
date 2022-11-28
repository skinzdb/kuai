#pragma once

#include "smpch.h"

namespace Smong {
	class Texture {
	public:
		Texture(const char* filename);
		~Texture();

		void Bind();
	private:
		uint32_t textureId;
	};
}