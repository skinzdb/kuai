#include "kuai.h"

#include <random>
#include <glm/gtx/string_cast.hpp>

struct ProfileResult
{
	const char* name;
	float time;
};

std::vector<ProfileResult> profileResults;

class BaseLayer : public kuai::Layer
{
public:
	std::shared_ptr<kuai::Entity> myEntity;
	std::shared_ptr<kuai::Entity> pointLight;

	float lastX = (float)kuai::App::get().getWindow().getWidth() / 2.0;
	float lastY = (float)kuai::App::get().getWindow().getHeight() / 2.0;
	bool firstMouse = true;

	float speed = 10.0f;
	float sensitivity = 0.1f;

	float counter = 0;

	std::vector<std::shared_ptr<kuai::Entity>> entities;

	BaseLayer() : Layer("Base"), scene(std::make_unique<kuai::Scene>())
	{
		KU_PROFILE_FUNCTION();

		//kuai::Mesh* mesh = new kuai::Mesh(quadPositions, quadNormals, quadTexCoords, quadIndices);
		auto whiteTex = std::make_shared<kuai::Texture>();

		std::vector<std::string> faces =
		{
			"C:/Users/David/Pictures/skybox/right.jpg",
			"C:/Users/David/Pictures/skybox/left.jpg",
			"C:/Users/David/Pictures/skybox/top.jpg",
			"C:/Users/David/Pictures/skybox/bottom.jpg",
			"C:/Users/David/Pictures/skybox/front.jpg",
			"C:/Users/David/Pictures/skybox/back.jpg"
		};

		auto cubemap = std::make_shared<kuai::Cubemap>(faces);
		auto skybox = std::make_shared<kuai::Skybox>(cubemap);

		auto skyboxEntity = scene->createEntity();
		skyboxEntity->addComponent<kuai::MeshRenderer>(skybox);
		
		auto grassTex = std::make_shared<kuai::Texture>("C:/Users/David/Pictures/grass.png");
		auto grassMat = std::make_shared<kuai::DefaultMaterial>(grassTex, grassTex, 10.0f);

		auto plane = std::make_shared<kuai::Model>("C:/Users/David/Documents/plane.obj");

		plane->getMeshes()[0]->setMaterial(grassMat);

		auto planeEntity = scene->createEntity();
		planeEntity->addComponent<kuai::MeshRenderer>(plane);
		planeEntity->getTransform().setPos(0, -1, -2);
		planeEntity->getTransform().setScale(1, 0.5f, 1);


		//auto model = std::make_shared<kuai::Model>("C:/Users/David/Documents/cs310/backpack/backpack.obj");
		auto model = std::make_shared<kuai::Model>("C:/Users/David/Documents/bunny.obj");
		auto modelMat = std::make_shared<kuai::DefaultMaterial>(whiteTex, whiteTex, 30.0f);
		modelMat->setReflection(cubemap);
		model->getMeshes()[0]->setMaterial(modelMat);
		
		myEntity = scene->createEntity();
		myEntity->addComponent<kuai::MeshRenderer>(model);
		myEntity->getTransform().translate(0, -1, -2);

		pointLight = scene->createEntity();
		pointLight->addComponent<kuai::Light>().setIntensity(0.5f);
		pointLight->getTransform().setPos(-1, 4, -2);

		//auto audio = std::make_shared<kuai::AudioClip>("C:/Users/David/Music/baka.wav");

		//myEntity->addComponent<kuai::AudioSourceComponent>(true);
		//auto a = myEntity->getComponent<kuai::AudioSourceComponent>();

		//a.source->setAudioClip(audio);
		//a.source->setPitch(0.5f);
		//a.source->setLoop(false);
		//a.source->play();

		/*std::default_random_engine generator;
		std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
		std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
		std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
		std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

		float scale = randScale(generator);

		auto cubemodel = kuai::Geometry::sphere();

		entities.resize(50);
		for (auto& entity : entities)
		{
			entity = scene->createEntity();

			entity->addComponent<kuai::MeshRenderer>(cubemodel);

			entity->getTransform().setPos(randPosition(generator), randPosition(generator), randPosition(generator));
			entity->getTransform().setRot(randRotation(generator), randRotation(generator), randRotation(generator));
			entity->getTransform().setScale(glm::vec3(scale, scale, scale));

		}*/

		/*for (int i = 0; i < 8; i++)
		{
			auto pointLight = scene->createEntity();
			pointLight->addComponent<kuai::Light>(1.0f, 0.02f, 0.005f);
			pointLight->getTransform().setPos(randPosition(generator), randPosition(generator), randPosition(generator));
			pointLight->getComponent<kuai::Light>().setCol(glm::vec3(abs(sin(i)), abs(cos(i)), (std::rand() % 10) / 10.0f));


		}*/

	}

	void update(float dt) override
	{
		KU_PROFILE_FUNCTION();
		scene->update(dt);

		float velocity = speed * dt;

		if (kuai::Input::isKeyPressed(kuai::Key::A))
		{
			scene->getMainCam().getTransform().translate(
				-scene->getMainCam().getTransform().getRight() * velocity);
		}
		if (kuai::Input::isKeyPressed(kuai::Key::D))
		{
			scene->getMainCam().getTransform().translate(
				scene->getMainCam().getTransform().getRight() * velocity);
		}
		if (kuai::Input::isKeyPressed(kuai::Key::W))
		{
			scene->getMainCam().getTransform().translate(
				scene->getMainCam().getTransform().getForward() * velocity);
		}
		if (kuai::Input::isKeyPressed(kuai::Key::S))
		{
			scene->getMainCam().getTransform().translate(
				-scene->getMainCam().getTransform().getForward() * velocity);
		}
		if (kuai::Input::isKeyPressed(kuai::Key::Space))
		{
			scene->getMainCam().getTransform().translate(0, 0.5f, 0);
		}
		if (kuai::Input::isKeyPressed(kuai::Key::LeftShift))
		{
			scene->getMainCam().getTransform().translate(0, -0.5f, 0);
		}


		if (kuai::Input::isKeyPressed(kuai::Key::R))
		{
			KU_CORE_WARN("{0},{1},{2}", scene->getMainLight().getTransform().getPos().x, scene->getMainLight().getTransform().getPos().y, scene->getMainLight().getTransform().getPos().z);

		    		scene->getMainLight().getTransform().translate(0, 0.1f, 0);
		}

				if (kuai::Input::isKeyPressed(kuai::Key::F))
		{
			KU_CORE_WARN("{0},{1},{2}", scene->getMainLight().getTransform().getPos().x, scene->getMainLight().getTransform().getPos().y, scene->getMainLight().getTransform().getPos().z);

		    		scene->getMainLight().getTransform().translate(0, -0.1f, 0);
		}

		counter += dt;
		
		//myEntity->getTransform().translate(0, 0, -0.05);
	}

	void onEvent(kuai::Event* event) override
	{
		if (event->getEventType() == kuai::EventType::MouseMove)
		{
			kuai::MouseMoveEvent* e = (kuai::MouseMoveEvent*)event;
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

			kuai::Transform t = scene->getMainCam().getTransform();
			scene->getMainCam().getTransform().rotate(yoffset, -xoffset, 0);

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (t.getRot().x > 89.0f) 
				t.setRot(89.0f, t.getRot().y, t.getRot().z);
			if (t.getRot().x < -89.0f)
				t.setRot(-89.0f, t.getRot().y, t.getRot().z);
		}
	}

private:
	std::unique_ptr<kuai::Scene> scene;
};

int main()
{
	kuai::Log::Init();

	KU_PROFILE_BEGIN_SESSION("startup", "kuai-profile-startup.json");
	kuai::App* app = new kuai::App();
	app->pushLayer(new BaseLayer());
	KU_PROFILE_END_SESSION();

	KU_PROFILE_BEGIN_SESSION("run", "kuai-profile-run.json");
	app->run();
	KU_PROFILE_END_SESSION();

	delete app;

	return 0;
}
