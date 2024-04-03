#pragma once

namespace kuai {
	struct Vertex
	{
		float pos[3];
		float normal[3];
		float tex_coords[2];
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
			 const std::vector<u32>& indices
		);

		Mesh(const std::vector<Vertex>& vertex_data, const std::vector<u32> indices);

		Mesh(const std::string& filename);

		virtual ~Mesh();

	private:
		u32 get_id() const { return mesh_id; }

	private:
		u32 mesh_id;

		std::vector<Vertex> vertex_data;
		std::vector<u32> indices;

		static u32 next_id;

		friend class Renderer;
	};
}

