#include "kpch.h"

#include "Renderer.h"

#include "rekuai/Core/Log.h"
#include "rekuai/Renderer/Buffer.h"
#include "rekuai/Renderer/RendererAPI.h"

namespace kuai
{
	std::unique_ptr<RendererAPI> Renderer::api = RendererAPI::create();
	std::unique_ptr<Renderer::SceneData> Renderer::scene_data = std::make_unique<SceneData>();

	void Renderer::init()
	{
		api->init();
	}

	void Renderer::stop()
	{
		api->stop();
	}

	void Renderer::cleanup()
	{
		api.reset(); // destruct render API before destructing GLFW
	}

	void Renderer::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		api->set_viewport(x, y, width, height);
	}

	void Renderer::set_camera(const Camera &camera)
	{
		scene_data->proj_matrix = camera.proj_matrix;
		scene_data->view_matrix = camera.view_matrix;
	}

	void Renderer::submit(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, const glm::mat4 &model)
	{
		api->clear();

		shader->bind();
		
		ModelViewProj mvp = {model, scene_data->view_matrix, scene_data->proj_matrix};
		shader->set_uniform_block("Matrices", "model", &mvp, sizeof(ModelViewProj));
		
		mesh->vertex_array->bind();
	
		api->draw_indexed(shader, mesh->vertex_array);
	}

	void Renderer::clear()
	{
		api->clear();
	}
}
