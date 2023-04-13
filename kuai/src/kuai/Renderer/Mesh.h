#pragma once

#include "Material.h"

namespace kuai {

	/** \class Mesh
	*	\brief A collection of vertices, normals and texture coordinates that define a polyhedral object. Each mesh has a material.
	*/
	class Mesh
	{
	public:
		/**
		* Constructs a mesh by specifying vertex data and indices.
		* @param positions Each three consecutive elements (starting at index 0) represent a vertex.
		* @param normals Each three consectuive elements (starting at index 0) represent a normal for the corresponding vertex.
		* @param texCoords Each two consecutive elements (starting at index 0) represent a texture coordinate for the corresponding vertex.
		* @param indices Vector of pointers that choose what vertices get drawn from the positions list.
		*/
		Mesh(const std::vector<float>& positions, 
			const std::vector<float>& normals, 
			const std::vector<float>& texCoords, 
			const std::vector<uint32_t>& indices,
			std::shared_ptr<Material> material);

		/**
		* Construct a mesh that uses the default material.
		*/
		Mesh(const std::vector<float>& positions, 
			const std::vector<float>& normals, 
			const std::vector<float>& texCoords, 
			const std::vector<uint32_t>& indices);

		virtual ~Mesh();
		// @private
		virtual void render();
		/**
		* Manually change the texture coordinates of this mesh.
		*/
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

