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

std::string read_file(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
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
        std::string vert_src = read_file("shader.vert");
        std::string frag_src = read_file("shader.frag");
        auto shader = Shader::create(vert_src, frag_src);
        shader->create_uniform_block("Matrices", {"model", "view", "proj"}, 0);

        cam = create_entity();
        cam->add_component<Transform>();
        cam->add_component<Camera>(
            60.0f,
            static_cast<float>(get_window().get_width()),
            static_cast<float>(get_window().get_height()),
            0.1f,
            100.0f
        );
        cam->get_component<Transform>().pos = glm::vec3(0.0f, 0.0f, 2.0f);

        auto mesh = std::make_shared<Mesh>(vertices, normals, tex_coords, indices);
        auto mesh2 = std::make_shared<Mesh>("cube.obj");

        test = create_entity();

        test->add_component<Transform>();
        test->add_component<MeshRenderer>(mesh2, std::make_shared<Material>(shader));

        test->get_component<Transform>().calc_model_matrix();
    }

    void update(float dt)
    {
        test->get_component<Transform>().rot += glm::vec3(0.0f, 0.5f * dt, 0.5f * dt);
        test->get_component<Transform>().calc_model_matrix();

    }

    void input(Event &e)
    {
    }

    ~MyApp()
    {
    }

private:
    std::shared_ptr<Entity> cam;
    std::shared_ptr<Entity> test;

};

App *kuai::create_app()
{
    return new MyApp();
}
