#pragma once

#include "rekuai/Renderer/Buffer.h"

namespace kuai
{
	struct Vertex
	{
		float pos[3];
		float normal[3];
		float tex_coords[2];

		bool operator==(const Vertex &other) const
		{
			return pos[0] == other.pos[0] && pos[1] == other.pos[1] && pos[2] == other.pos[2] &&
				   normal[0] == other.normal[0] && normal[1] == other.normal[1] && normal[2] == other.normal[2] &&
				   tex_coords[0] == other.tex_coords[0] && tex_coords[1] == other.tex_coords[1];
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
		Mesh(const std::vector<float> &positions,
			 const std::vector<float> &normals,
			 const std::vector<float> &tex_coords,
			 const std::vector<uint32_t> &indices);
		Mesh(const std::string &filename);
		~Mesh();

		uint32_t get_id() { return vertex_array->get_id(); }

	private:
		void init(const std::vector<Vertex> &vertex_data, const std::vector<uint32_t> &indices);

		std::shared_ptr<VertexArray> vertex_array;

		friend class Renderer;
	};
}
