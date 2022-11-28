#pragma once

namespace Smong {
	class Mesh
	{
	public:
		Mesh(float positions[], uint32_t posCount, float texCoords[], uint32_t texCount, uint32_t indices[], uint32_t vertCount);
		~Mesh();

		void Render();

	private:
		uint32_t vaoId;
		uint32_t vboIds[3] = { 0,0,0 };
		uint32_t vertCount;
	};
}

