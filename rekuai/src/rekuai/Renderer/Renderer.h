#pragma once

#include "glm/glm.hpp"

#include "Framebuffer.h"
#include "Mesh.h"
#include "Material.h"

#include "rekuai/Components/Components.h"

namespace kuai {

	enum RenderFlag : int
	{
		Never = 0x200,
		Less = 0x201,
		Equal = 0x202,
		LEqual = 0x203,
		Greater = 0x204,
		NotEqual = 0x205,
		GEqual = 0x206,
		Always = 0x207,

		Depth = 0x00000100,
		Col = 0x00000400,
		Stencil = 0x00004000
	};

	struct RenderState
	{
		RenderFlag depth_test = RenderFlag::Less;
		int clear_flags = RenderFlag::Col | RenderFlag::Depth;
		// uniforms?
		// textures?
	};

	struct ShaderData
	{
		std::unordered_map<u32, IndirectCommand> mesh_to_cmd;
		u32 instances;
	};

	struct OffsetData
	{
		size_t vertices_offset;
		size_t indices_offset;
	};

	struct RendererData
	{
		 

		std::vector<Transform> transforms;
		std::vector<Shader> shaders;
		
		std::unordered_map<u32, ShaderData> shader_map;	// Maps shader ID to its commands and total number of instances
		std::unordered_map<u32, OffsetData> offset_map;	// Maps mesh ID to its vertex and index offsets

		std::vector<Vertex> vertex_data;
		std::vector<u32> indices;
	};

	class Renderer
	{
	public:
		static void init();
		static void cleanup();

		static void add_object(const MeshRenderer& m_renderer, Transform& transform);
		static void remove_object(const MeshRenderer& m_renderer, Transform& transform);

		static void set_camera(const Camera& camera);

		static void update();

		static void set_viewport(u32 x, u32 y, u32 width, u32 height);
		static void set_clear_col(const glm::vec4& col);
		static void clear();

	private:
		static RendererData r_data;
	};
}

