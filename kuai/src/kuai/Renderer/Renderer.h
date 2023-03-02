#pragma once

#include "kuai/Components/Entity.h"
#include "kuai/Components/Components.h"

#include "glm/glm.hpp"

namespace kuai {
	class Renderer
	{
	public:
		static void init();
		static void cleanup();

		static void setCamera(Camera& cam, glm::vec3& viewPos);
		static void setLights(std::vector<std::shared_ptr<Entity>>& lights);
		static void render(MeshMaterial& meshMat, glm::mat4& transform);

		static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void setClearCol(const glm::vec4& col);
		static void clear();

	private:
		struct CameraData
		{
			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
			glm::vec3 viewPos;
		};

		static std::unique_ptr<CameraData> camData;

		static std::vector<std::shared_ptr<Entity>> lights;
	};
}

