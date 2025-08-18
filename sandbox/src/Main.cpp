using namespace std;

#include "kuai.h"

#include "glm/glm.hpp"

using namespace kuai;

// const char* vert_src = R"(
// 	#version 120

// 	layout (location = 0)	in vec3 aPos;
// 	layout (location = 1)	in vec3 aNormal;
// 	layout (location = 2)	in vec2 aTexCoord;
// 	layout (location = 3)	in mat4 aModelMatrix;


// 	layout (binding = 0) uniform CamData
// 	{
// 		mat4 projectionMatrix;
// 		mat4 viewMatrix;
// 		vec3 viewPos;
// 	};

// 	out vec4 world_pos;
// 	out vec3 world_norm;
// 	out vec2 tex_coords;
// 	out vec3 viewingPos;

// 	void main()
// 	{
// 		world_pos = aModelMatrix * vec4(aPos, 1.0);
// 		mat3 model3x3InvTransp = mat3(transpose(inverse(aModelMatrix))); // TODO: remove in favour of vertex attribute
// 		world_norm = model3x3InvTransp * aNormal;
// 		tex_coords = aTexCoord;
// 		viewingPos = viewPos;

// 		gl_Position = projectionMatrix * viewMatrix * world_pos;
// 	}
// )";

// const char* frag_src = R"(
// 	#version 120

// 	in vec4 world_pos;
// 	in vec3 world_norm;
// 	in vec2 tex_coords;

// 	in flat float tex_index;
// 	in flat float tiling;

// 	uniform sampler2DArray sprites;

// 	out vec4 fragCol;

// 	void main()
// 	{
// 		fragCol = texture(sprites, vec3(tex_coords * tiling, int(tex_index)));
// 	}
// )";


class MyApp : public App
{
public:
	MyApp()
	{
		// scene = new Scene();
		// Entity cam = scene->create_entity();
		// Entity test = scene->create_entity();

		// cam.add_component<Camera>(60, get_window().get_width(), get_window().get_height(), 0.1f, 100.0f);

		// Shader shader = Shader(vert_src, frag_src);
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
	Scene* scene;
};

App* kuai::create_app()
{
	return new MyApp();
}
