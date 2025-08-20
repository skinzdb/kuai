#pragma once

#include "rekuai/Renderer/Buffer.h"

namespace kuai {
	struct Vertex
	{
		float pos[3];
		float normal[3];
		float tex_coords[2];

  		bool operator<(const kuai::Vertex& other) const {
            return pos < other.pos;
        }
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
		* @param tex_coords Each two consecutive elements (starting at index 0) represent a texture coordinate for the corresponding vertex.
		* @param indices Vector of pointers that choose what vertices get drawn from the positions list.
		*/
		Mesh(const std::vector<float>& positions,
			 const std::vector<float>& normals,
			 const std::vector<float>& tex_coords,
			 const std::vector<uint32_t>& indices
		);

		Mesh(const std::string& filename);

		Mesh(const std::vector<Vertex>& vertex_data, const std::vector<uint32_t>& indices);

		virtual ~Mesh();

	private:
	    std::unique_ptr<VertexArray> vertex_array;
	};
}
