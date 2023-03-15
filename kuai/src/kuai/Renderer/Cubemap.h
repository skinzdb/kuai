#pragma once

namespace kuai {
	class Cubemap
	{
	public:
		// Order of faces: px, nx, py, ny, pz, nz
		Cubemap(const std::vector<std::string>& faces);
		~Cubemap();

		uint32_t getId() { return textureId; }

		void bind();
	private:
		uint32_t textureId;
	};

}

