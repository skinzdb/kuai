#pragma once

#include "glm/glm.hpp"

#include "Framebuffer.h"
#include "Mesh.h"
#include "Material.h"

#include "RendererAPI.h"
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
		// std::unordered_map<uint32_t, IndirectCommand> mesh_to_cmd;
		uint32_t instances;
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

		std::unordered_map<uint32_t, ShaderData> shader_map;	// Maps shader ID to its commands and total number of instances
		std::unordered_map<uint32_t, OffsetData> offset_map;	// Maps mesh ID to its vertex and index offsets

		std::vector<Vertex> vertex_data;
		std::vector<uint32_t> indices;
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

		static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void set_clear_col(const glm::vec4& col);
		static void clear();

	private:
		static std::unique_ptr<RendererAPI> api;
		static RendererData r_data;
	};
}
