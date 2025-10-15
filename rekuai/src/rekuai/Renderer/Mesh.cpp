#include "Mesh.h"
#include "rekuai/Renderer/Buffer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include "tiny_obj_loader.h"
#include <unordered_map>

namespace std {
   template<> struct hash<kuai::Vertex> {
       size_t operator()(kuai::Vertex const& vert) const {
           return hash<glm::mat3>()(glm::mat3(
               vert.pos[0], vert.pos[1], vert.pos[2],
               vert.normal[0], vert.normal[1], vert.normal[2],
               vert.tex_coords[0], vert.tex_coords[1], 0
           ));
       }
   };
}

namespace kuai {
    void Mesh::init(const std::vector<Vertex>& vertex_data, const std::vector<uint32_t>& indices)
	{
        auto vertex_buf = VertexBuffer::create(sizeof(Vertex) * vertex_data.size());
		vertex_buf->set_layout(BufferLayout {
            BufferElement(ShaderDataType::VEC3, "positions"),
            BufferElement(ShaderDataType::VEC3, "normals"),
            BufferElement(ShaderDataType::VEC2, "tex_coords")
		});
		vertex_buf->set_data(vertex_data.data(), sizeof(Vertex) * vertex_data.size());

		vertex_array = VertexArray::create();
		vertex_array->add_vertex_buffer(vertex_buf);

		auto index_buf = IndexBuffer::create(indices.data(), indices.size());
		vertex_array->set_index_buffer(index_buf);
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

		init(vertex_data, indices);
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
			return;
		}
		if (!warn.empty())
		{
			KU_CORE_WARN(warn);
			return;
		}

		std::vector<Vertex> vertex_data{};
		std::vector<uint32_t> indices{};
		std::unordered_map<Vertex, uint32_t> unique_vertices{};

		for (int i = 0; i < shapes.size(); i ++) {
            tinyobj::shape_t &shape = shapes[i];
            tinyobj::mesh_t &mesh = shape.mesh;

            for (int j = 0; j < mesh.indices.size() / 3; j++) {
                tinyobj::index_t idx = mesh.indices[j];

                float pos[3] = {
                    attrib.vertices[idx.vertex_index * 3],
                    attrib.vertices[idx.vertex_index * 3 + 1],
                    attrib.vertices[idx.vertex_index * 3 + 2]
                };

                float normal[3];
                if (idx.normal_index >= 0)
                {
                    normal[0] = attrib.normals[idx.normal_index * 3];
                    normal[1] = attrib.normals[idx.normal_index * 3 + 1];
                    normal[2] = attrib.normals[idx.normal_index * 3 + 2];
                }

                float tex_coords[2];
                if (idx.texcoord_index >= 0)
                {
                    tex_coords[0] = attrib.texcoords[idx.texcoord_index * 2];
                    tex_coords[1] = attrib.texcoords[idx.texcoord_index * 2 + 1];
                }

                auto vert = Vertex {
                    pos[0], pos[1], pos[2],
                    normal[0], normal[1], normal[2],
                    tex_coords[0], tex_coords[1]
                };

               	if (unique_vertices.count(vert) == 0) {
				    unique_vertices[vert] = static_cast<uint32_t>(vertex_data.size());
					vertex_data.push_back(vert);
				}

				indices.push_back(unique_vertices[vert]);
            }
	    }

		init(vertex_data, indices);
	}

	Mesh::~Mesh()
	{
	}
}
