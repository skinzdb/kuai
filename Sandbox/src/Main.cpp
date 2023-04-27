#include "kuai.h"

#include <random>
#include <glm/gtx/string_cast.hpp>

using namespace kuai;

struct ProfileResult
{
	const char* name;
	float time;
};

std::vector<ProfileResult> profileResults;

class BaseLayer : public Layer
{
public:
	Rc<Entity> myEntity;
	Rc<Entity> pointLight;

	float lastX = (float)App::get().getWindow().getWidth() / 2.0;
	float lastY = (float)App::get().getWindow().getHeight() / 2.0;
	bool firstMouse = true;

	float speed = 10.0f;
	float sensitivity = 0.1f;

	float counter = 0;

	std::vector<Rc<Entity>> entities;

	BaseLayer() : Layer("Base"), scene(std::make_unique<Scene>())
	{
		KU_PROFILE_FUNCTION();

		auto whiteTex = MakeRc<Texture>();

		//myEntity = scene->createEntity();
		//Rc<Mesh> mesh = MakeRc<Mesh>(positions, std::vector<float>(), std::vector<float>(), indices);
		//Rc<Material> mat = MakeRc<TestMaterial>();
		//Rc<Model> model = MakeRc<Model>(mesh, mat);
		//myEntity->getTransform().setPos(0, 0, -5);
		//myEntity->addComponent<MeshRenderer>(model);

		////std::vector<std::string> faces =
		////{
		////	"C:/Users/David/Pictures/skybox/right.jpg",
		////	"C:/Users/David/Pictures/skybox/left.jpg",
		////	"C:/Users/David/Pictures/skybox/top.jpg",
		////	"C:/Users/David/Pictures/skybox/bottom.jpg",
		////	"C:/Users/David/Pictures/skybox/front.jpg",
		////	"C:/Users/David/Pictures/skybox/back.jpg"
		////};

		////auto cubemap = MakeRc<Cubemap>(faces);
		////auto skybox = MakeRc<Skybox>(cubemap);

		////auto skyboxEntity = scene->createEntity();
		////skyboxEntity->addComponent<MeshRenderer>(skybox);
		//
		//auto grassTex = MakeRc<Texture>("C:/Users/David/Pictures/grass.png");
		//auto grassMat = MakeRc<DefaultMaterial>(grassTex, grassTex, 10.0f);

		//auto plane = MakeRc<Model>("C:/Users/David/Documents/plane.obj");

		//plane->getMaterials()[0] = grassMat;

		//auto planeEntity = scene->createEntity();
		//planeEntity->addComponent<MeshRenderer>(plane);
		//planeEntity->getTransform().setPos(0, -1, -2);
		//planeEntity->getTransform().setScale(1, 0.5f, 1);

		//auto model = MakeRc<Model>("C:/Users/David/Documents/cs310/backpack/backpack.obj");
		auto model = MakeRc<Model>("C:/Users/David/Documents/bunny.obj");
		auto modelMat = MakeRc<DefaultMaterial>(whiteTex, whiteTex, 30.0f);
		//modelMat->setReflection(cubemap);
		model->getMaterials()[0] = modelMat;
		
		myEntity = scene->createEntity();
		myEntity->getTransform().translate(0, -1, -2);

		myEntity->addComponent<MeshRenderer>(model);

		//pointLight = scene->createEntity();
		//pointLight->addComponent<Light>().setIntensity(0.5f);
		//pointLight->getTransform().setPos(-1, 4, -2);

		//auto audio = MakeRc<AudioClip>("C:/Users/David/Music/baka.wav");

		//myEntity->addComponent<AudioSourceComponent>(true);
		//auto a = myEntity->getComponent<AudioSourceComponent>();

		//a.source->setAudioClip(audio);
		//a.source->setPitch(0.5f);
		//a.source->setLoop(false);
		//a.source->play();

		auto cubeModel = MakeRc<Model>("C:/Users/David/Documents/cube.obj");
		cubeModel->getMaterials()[0] = modelMat;

		std::default_random_engine generator;
		std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
		std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
		std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
		std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

		float scale = randScale(generator);

		entities.resize(10000);
		for (auto& entity : entities)
		{
			entity = scene->createEntity();

			entity->getTransform().setPos(randPosition(generator), randPosition(generator), randPosition(generator));
			entity->getTransform().setRot(randRotation(generator), randRotation(generator), randRotation(generator));
			entity->getTransform().setScale(glm::vec3(0.5f, 0.5f, 0.5f));

			entity->addComponent<MeshRenderer>(cubeModel);
		}

		/*for (int i = 0; i < 8; i++)
		{
			auto pointLight = scene->createEntity();
			pointLight->addComponent<Light>(1.0f, 0.02f, 0.005f);
			pointLight->getTransform().setPos(randPosition(generator), randPosition(generator), randPosition(generator));
			pointLight->getComponent<Light>().setCol(glm::vec3(abs(sin(i)), abs(cos(i)), (std::rand() % 10) / 10.0f));


		}*/

	}

	void update(float dt) override
	{
		KU_PROFILE_FUNCTION();
		scene->update(dt);

		float velocity = speed * dt;

		if (Input::isKeyPressed(Key::A))
		{
			scene->getMainCam().getTransform().translate(
				-scene->getMainCam().getTransform().getRight() * velocity);
		}
		if (Input::isKeyPressed(Key::D))
		{
			scene->getMainCam().getTransform().translate(
				scene->getMainCam().getTransform().getRight() * velocity);
		}
		if (Input::isKeyPressed(Key::W))
		{
			scene->getMainCam().getTransform().translate(
				scene->getMainCam().getTransform().getForward() * velocity);
		}
		if (Input::isKeyPressed(Key::S))
		{
			scene->getMainCam().getTransform().translate(
				-scene->getMainCam().getTransform().getForward() * velocity);
		}
		if (Input::isKeyPressed(Key::Space))
		{
			scene->getMainCam().getTransform().translate(0, 0.5f, 0);
		}
		if (Input::isKeyPressed(Key::LeftShift))
		{
			scene->getMainCam().getTransform().translate(0, -0.5f, 0);
		}

		if (entities.size())
		{
			counter = 0.0f;
			entities.back()->destroy();
			entities.back().reset();
			entities.pop_back();
		}
		counter += dt;
		
		
		//myEntity->getTransform().translate(0, 0, -0.05);
	}

	void onEvent(Event* event) override
	{
		if (event->getEventType() == EventType::MouseMove)
		{
			MouseMoveEvent* e = (MouseMoveEvent*)event;
			float xpos = static_cast<float>(e->getX());
			float ypos = static_cast<float>(e->getY());
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

			lastX = xpos;
			lastY = ypos;

			xoffset *= sensitivity; // yaw
			yoffset *= sensitivity; // pitch

			Transform t = scene->getMainCam().getTransform();
			scene->getMainCam().getTransform().rotate(yoffset, -xoffset, 0);

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (t.getRot().x > 89.0f) 
				t.setRot(89.0f, t.getRot().y, t.getRot().z);
			if (t.getRot().x < -89.0f)
				t.setRot(-89.0f, t.getRot().y, t.getRot().z);
		}
	}

private:
	Box<Scene> scene;
};

int main()
{
	Log::Init();

	KU_PROFILE_BEGIN_SESSION("startup", "kuai-profile-startup.json");
	App* app = new App();
	App::get().getWindow().setVSync(false);
	app->pushLayer(new BaseLayer());
	KU_PROFILE_END_SESSION();

	KU_PROFILE_BEGIN_SESSION("run", "kuai-profile-run.json");
	app->run();
	KU_PROFILE_END_SESSION();



	delete app;

	return 0;
}
