#include "Mesh.h"

#include "tiny_obj_loader.h"

namespace kuai {
    Mesh::Mesh(const std::vector<Vertex>& vertex_data, const std::vector<uint32_t>& indices) 
	{
        auto vertex_buf = VertexBuffer::create(vertex_data.size());
		vertex_buf->set_layout(BufferLayout {
            BufferElement(ShaderDataType::VEC3, "positions"),
            BufferElement(ShaderDataType::VEC3, "normals"),
            BufferElement(ShaderDataType::VEC2, "tex_coords")
		});
		vertex_buf->set_data(&vertex_data[0], vertex_data.size());
		vertex_array->add_vertex_buffer(std::move(vertex_buf));

		auto index_buf = IndexBuffer::create(&indices[0], indices.size());
		vertex_array->set_index_buffer(std::move(index_buf));
    }

	Mesh::Mesh(const std::vector<float>& positions,
	           const std::vector<float>& normals,
			   const std::vector<float>& tex_coords,
			   const std::vector<uint32_t>& indices)
	{
		uint32_t vert_count = positions.size() / 3;

		auto vertex_data = std::vector<Vertex>(vert_count);
		bool use_normals = positions.size() == normals.size();

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

		Mesh(vertex_data, indices);
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

		auto vertex_data = std::vector<Vertex>();
		std::map<Vertex, uint32_t> unique_vertices{};
		auto indices = std::vector<uint32_t>();

		for (const auto& shape : shapes) {
            for (const auto& idx : shape.mesh.indices) {
				Vertex vert = Vertex {
				  attrib.vertices[3 * idx.vertex_index + 0],
			      attrib.vertices[3 * idx.vertex_index + 1],
				  attrib.vertices[3 * idx.vertex_index + 2],
				  attrib.normals[3 * idx.normal_index + 0],
				  attrib.normals[3 * idx.normal_index + 1],
				  attrib.normals[3 * idx.normal_index + 2],
			      attrib.texcoords[2 * idx.texcoord_index + 0],
			      attrib.texcoords[2 * idx.texcoord_index + 1]
				};

				if (unique_vertices.count(vert) == 0) {
				    unique_vertices[vert] = static_cast<uint32_t>(vertex_data.size());
					vertex_data.push_back(vert);
				}

				indices.push_back(unique_vertices[vert]);
			}
		}

		Mesh(vertex_data, indices);
	}

	Mesh::~Mesh()
	{
	}
}
