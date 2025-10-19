#include "kuai.h"

using namespace kuai;

const std::vector<float> vertices = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

const std::vector<float> normals = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f
};

const std::vector<float> tex_coords = {
    0.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 0.0f
};

const std::vector<uint32_t> indices = {
    0, 1, 2, 2, 3, 0
};

std::string read_file(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    KU_ERROR("Failed to open file {}", filename);
    return "";
  }

  size_t file_size = (size_t)file.tellg();
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
        // Shader object creation
        std::string vert_src = read_file("vert.spv");
        std::string frag_src = read_file("frag.spv");
        auto shader = Shader::create(vert_src, frag_src);

//		auto cam = scene->create_entity();
		auto test = create_entity();

		// cam->add_component<Camera>(60, get_window().get_width(), get_window().get_height(), 0.1f, 100.0f);

		auto mesh = std::make_shared<Mesh>(vertices, normals, tex_coords, indices);

		test->add_component<Transform>();
		test->add_component<MeshRenderer>(mesh, std::make_shared<Material>(shader));

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

	~MyApp()
	{

	}

private:
};

App* kuai::create_app()
{
	return new MyApp();
}
