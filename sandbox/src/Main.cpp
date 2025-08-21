#include "rekuai/Core/Log.h"
using namespace std;

#include "kuai.h"

#include "glm/glm.hpp"

using namespace kuai;

std::string read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        KU_CORE_ERROR("Failed to open file {}", filename);
        return "";
    }

    size_t file_size = (size_t) file.tellg();
    std::vector<char> buffer(file_size);

    file.seekg(0);
    file.read(buffer.data(), file_size);

    file.close();

    return std::string(buffer.begin(), buffer.end());
}

class MyApp : public App
{
public:

	MyApp()
	{
		scene = make_unique<Scene>();
		Entity cam = scene->create_entity();
		Entity test = scene->create_entity();

		cam.add_component<Camera>(60, get_window().get_width(), get_window().get_height(), 0.1f, 100.0f);

		auto vert_code = read_file("vert.spv");
		auto frag_code = read_file("frag.spv");

		auto shader = Shader::create(vert_code, frag_code);
		// Mesh mesh = Mesh("C:/Users/David/Documents/cube.obj");
		// //auto& material = Material(shader);

		// test.add_component<Transform>();
		//test.add_component<MeshRenderer>(mesh, material);


		//default material

		///*signal sketch*/

		//
		//
		//fuck = ecs->create_signal<int(int)>();
		//
		//fuc
		//


		//
		//view.each([&fuck](float dt, PlayerComponent comp) {
		//	if (dt > 100) {
		//		fuck->publish(65);
		//	}
		//});


		//
		//

		//view->each([](float dt, Transform& transform) {
		//	transform.pos += glm::vec3(dt, 0, 0);
		//});

	}

	void update(float dt)
	{

	}

	void input(Event& e)
	{

	}

private:
	unique_ptr<Scene> scene;
};

App* kuai::create_app()
{
	return new MyApp();
}
