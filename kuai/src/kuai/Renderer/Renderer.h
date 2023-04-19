#pragma once

#include "kuai/Components/Entity.h"
#include "kuai/Components/Components.h"

#include "Framebuffer.h"

#include "glm/glm.hpp"

namespace kuai {

	static const uint32_t SHADOW_MAP_SIZE = 2048;	// Total square size of shadow map
	static const uint32_t LIGHT_SHADOW_SIZE = 2048;  // Square size of each light in the shadow map
	static const uint32_t LIGHTS_PER_ROW = SHADOW_MAP_SIZE / LIGHT_SHADOW_SIZE;

	class Renderer
	{
	public:
		static void init();
		static void cleanup();

		static void setCamera(CameraComponent& camera);
		
		static void updateShadowMap(Light light);
		static void setLights(const std::vector<std::shared_ptr<Entity>>& lightEntities);
		static void setMeshes(const std::vector<std::shared_ptr<Entity>>& renderEntities);

		static void render();

		static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void setClearCol(const glm::vec4& col);
		static void clear();

	private:
		static void renderDepth();

		struct RenderData
		{
			std::vector<std::shared_ptr<Entity>> renderEntities;
			std::vector<std::shared_ptr<Entity>> lightEntities;
		};

		static std::unique_ptr<RenderData> renderData;

		static std::shared_ptr<Framebuffer> framebuffer;
		static std::unique_ptr<Framebuffer> shadowMap;
	};
}

