using namespace std;

#include "kuai.h"

#include "glm/glm.hpp"

using namespace kuai;

class MyApp : public App 
{
public:
	MyApp() : scene(new Scene())
	{
		auto& test = scene->create_entity();
		
		auto& mesh = Mesh("C:/Users/David/Documents/cube.obj");
		auto& material = Material();

		test.add_component<Transform>();
		test.add_component<MeshRenderer>(mesh, material);
		

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