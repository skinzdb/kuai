using namespace std;

#include "kuai.h"

#include "glm/glm.hpp"

using namespace kuai;

class MyApp : public App 
{
public:
	MyApp()
	{
		auto& view = ecs->register_system<Transform>();
		EntityId e = ecs->create_entity();
		ecs->add_component<Transform>(e);

		view->each([](float dt, Transform& transform) {
			transform.pos += glm::vec3(dt, 0, 0);
		});

	}

	void update(float dt) 
	{
		ecs->update(dt);
	}

	void input(Event& e)
	{

	}
};

App* kuai::create_app() 
{
	return new MyApp();
}