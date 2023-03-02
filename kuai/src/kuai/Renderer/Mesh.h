#pragma once

namespace kuai {
	class Mesh
	{
	public:
		Mesh(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<uint32_t>& indices);
		~Mesh();

		void render();

	private:
		uint32_t vaoId;
		uint32_t vboIds[4] = { 0, 0, 0, 0 };
		uint32_t vertCount;
	};
}

