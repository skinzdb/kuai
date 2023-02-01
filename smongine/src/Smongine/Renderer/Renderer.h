#pragma once

#include "Smongine/Components/Entity.h"
#include "Smongine/Components/Components.h"

#include "glm/glm.hpp"

namespace Smong {
	class Renderer
	{
	public:
		static void Init();
		static void Cleanup();

		static void SetCamera(Camera& cam, glm::vec3& viewPos);
		static void SetLights(std::vector<std::shared_ptr<Entity>>& lights);
		static void Render(MeshMaterial& meshMat, glm::mat4& transform);

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearCol(const glm::vec4& col);
		static void Clear();

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

