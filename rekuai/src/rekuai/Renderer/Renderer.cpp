#include "Renderer.h"

#include "glad/glad.h"

namespace kuai {

	RendererData Renderer::r_data = RendererData();

	void Renderer::init()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::cleanup()
	{

	}

	void Renderer::add_object(const MeshRenderer& m_renderer, Transform& transform)
	{
		auto& s_data = r_data.shader_map[m_renderer.material.shader];
		auto& mesh = m_renderer.mesh;

		r_data.transforms.push_back(transform);

		if (r_data.offset_map.find(m_renderer.mesh.id) == r_data.offset_map.end()) // New mesh data => record offsets and add it to vertex_data and indices
		{
			r_data.offset_map[mesh.id].vertices_offset = r_data.vertex_data.size();
			r_data.offset_map[mesh.id].indices_offset = r_data.indices.size();

			r_data.vertex_data.insert(r_data.vertex_data.end(), mesh.vertex_data.begin(), mesh.vertex_data.end());
			r_data.indices.insert(r_data.indices.end(), mesh.indices.begin(), mesh.indices.end());
		}

		IndirectCommand& cmd = s_data.mesh_to_cmd[mesh.id];

		if (cmd.inst_count == 0)
		{
			cmd.count = mesh.indices.size();							     // Number of indices mesh uses	
			cmd.first_idx = r_data.offset_map[mesh.id].indices_offset;	     // Offset of first index
			cmd.base_vertex = r_data.offset_map[mesh.id].vertices_offset;	 // Offset of first vertex
			cmd.base_inst = s_data.instances;								 // Offset of first instance
		}

		cmd.inst_count++;
		s_data.instances++;
	}

	void Renderer::remove_object(const MeshRenderer& m_renderer, Transform& transform)
	{
		auto& s_data = r_data.shader_map[m_renderer.material.shader];

		r_data.transforms.erase(std::remove(r_data.transforms.begin(), r_data.transforms.end(), transform), r_data.transforms.end());

		s_data.mesh_to_cmd[m_renderer.mesh.id].inst_count--;
		s_data.instances--;

		// TODO: vertex data and indices will never get deleted from list

		// Decrement base instances of all meshes that are further along the list as we deleted an instance 
		for (auto& [_, cmd] : s_data.mesh_to_cmd)
		{
			if (cmd.base_inst > cmd.base_inst)
			{
				cmd.base_inst--;
			}
		}

		if (s_data.instances == 0)
		{
			r_data.shader_map.erase(m_renderer.material.shader);
		}
	}

	void Renderer::set_camera(const Camera& camera)
	{
	}

	void Renderer::update()
	{
		Shader::set_uniform("proj_matrix", )

		for (auto& [shader, _] : r_data.shader_map)
		{
			shader.bind();

			size_t cmd_count = r_data.shader_map[shader].mesh_to_cmd.size();

			glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (void*)0, cmd_count, sizeof(IndirectCommand));
		}
	}

	void Renderer::set_viewport(u32 x, u32 y, u32 width, u32 height)
	{
		glViewport(x, y, width, height);
	}

	void Renderer::set_clear_col(const glm::vec4& col)
	{
		glClearColor(col.r, col.g, col.b, col.a);
	}

	void Renderer::clear()
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}

