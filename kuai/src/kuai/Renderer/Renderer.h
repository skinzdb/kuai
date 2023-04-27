#pragma once

#include "Buffer.h"
#include "Framebuffer.h"

#include "kuai/Components/Components.h"

#include "glm/glm.hpp"

namespace kuai {

	static const u32 SHADOW_MAP_SIZE = 1024;	// Total square size of shadow map
	static const u32 LIGHT_SHADOW_SIZE = 1024;  // Square size of each light in the shadow map
	static const u32 LIGHTS_PER_ROW = SHADOW_MAP_SIZE / LIGHT_SHADOW_SIZE;

	class Renderer
	{
	public:
		static void init();
		static void cleanup();
		
		static void setCamera(Camera& camera);

		static void updateShadowMap(Light& light);

		static void render(Shader& shader);

		static void setViewport(u32 x, u32 y, u32 width, u32 height);
		static void setClearCol(const glm::vec4& col);
		static void clear();

	private:
		static void renderDepth();

	private:
		struct RenderData
		{
			int shouldthisbeused;
		};

		static Box<RenderData> renderData;

		static Rc<Framebuffer> framebuffer;
		static Box<Framebuffer> shadowMap;
	};
}

