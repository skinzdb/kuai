#pragma once

#include "Material.h"

namespace kuai {
	class Mesh
	{
	public:
		Mesh(const std::vector<float>& positions, 
			const std::vector<float>& normals, 
			const std::vector<float>& texCoords, 
			const std::vector<uint32_t>& indices,
			std::shared_ptr<Material> material);

		Mesh(const std::vector<float>& positions, 
			const std::vector<float>& normals, 
			const std::vector<float>& texCoords, 
			const std::vector<uint32_t>& indices);

		virtual ~Mesh();

		virtual void render();

		void setTexCoords(std::vector<float>& texCoords);

		std::shared_ptr<Material> getMaterial() { return material; }
		void setMaterial(std::shared_ptr<Material> material) { this->material = material; }

	protected:
		uint32_t vaoId;
		uint32_t vboIds[4] = { 0, 0, 0, 0 };
		uint32_t vertCount;

		std::shared_ptr<Material> material;
	};
}

