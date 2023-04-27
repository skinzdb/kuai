#pragma once

#include "Material.h"
#include "Buffer.h"

namespace kuai {
	struct Vertex
	{
		float pos[3];
		float normal[3];
		float texCoords[2];
		u32 texId;
	};

	/** \class Mesh
	*	\brief A collection of vertices, normals and texture coordinates that define a polyhedral object. Each mesh has a Material.
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
			const std::vector<u32>& indices
		);

		Mesh(const std::vector<Vertex>& vertexData, const std::vector<u32> indices);

		virtual ~Mesh();

	private:
		u32 getId() const { return meshId; }

		friend class RenderSystem;

	private:
		u32 meshId;

		std::vector<Vertex> vertexData;
		std::vector<u32> indices;

	private:
		static u32 meshCounter;
	};
}

