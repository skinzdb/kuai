#include "Mesh.h"

#include "tiny_obj_loader.h"

namespace kuai {

	uint32_t Mesh::next_id = 0;

	Mesh::Mesh(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& tex_coords, const std::vector<uint32_t>& indices)
	{
		uint32_t vert_count = positions.size() / 3;

		vertex_data.resize(vert_count);
		bool use_normals = positions.size() == normals.size();

		this->indices = indices;

		for (size_t i = 0; i < vert_count; i++)
		{
			vertex_data[i].pos[0] = positions[i * 3];
			vertex_data[i].pos[1] = positions[i * 3 + 1];
			vertex_data[i].pos[2] = positions[i * 3 + 2];

			if (use_normals)
			{
				vertex_data[i].normal[0] = normals[i * 3];
				vertex_data[i].normal[1] = normals[i * 3 + 1];
				vertex_data[i].normal[2] = normals[i * 3 + 2];
			}

			if (i < tex_coords.size())
			{
				vertex_data[i].tex_coords[0] = tex_coords[i * 2];
				vertex_data[i].tex_coords[1] = tex_coords[i * 2 + 1];
			}
		}
		id = next_id++;
	}

	Mesh::Mesh(const std::string& filename)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;
		std::string warn;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &warn, filename.c_str());

		if (!err.empty())
		{
			KU_CORE_ERROR(err);
		}
		if (!warn.empty())
		{
			KU_CORE_WARN(warn);
		}

		for (size_t s = 0; s < shapes.size(); s++)
		{
			// Loop over faces (polygon)
			size_t idx_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face
				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[idx_offset + v];
					vertex_data.emplace_back(
						Vertex
						{ attrib.vertices[3 * idx.vertex_index + 0],
					      attrib.vertices[3 * idx.vertex_index + 1],
						  attrib.vertices[3 * idx.vertex_index + 2],
						  attrib.normals[3 * idx.normal_index + 0],
						  attrib.normals[3 * idx.normal_index + 1],
						  attrib.normals[3 * idx.normal_index + 2],
					      attrib.texcoords[2 * idx.texcoord_index + 0],
					      attrib.texcoords[2 * idx.texcoord_index + 1]
						}
					);
				}
				idx_offset += fv;
				// Per-face material
				// shapes[s].mesh.material_ids[f];
			}
		}
		id = next_id++;
	}

	Mesh::~Mesh()
	{
	}
}
