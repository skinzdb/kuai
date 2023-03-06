#include <kuai.h>

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
	std::vector<float> quadPositions = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	std::vector<float> quadNormals = {
		0, 0, 1
	};

	std::vector<float> quadTexCoords = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};

	std::vector<uint32_t> quadIndices = { 0, 1, 2, 2, 3, 0 };

	std::shared_ptr<kuai::Entity> cube;
	std::shared_ptr<kuai::Entity> light;

	float num = 0;


	std::vector<std::shared_ptr<kuai::Entity>> entities;

	BaseLayer() : Layer("Base"), scene(std::make_unique<kuai::Scene>())
	{
		KU_PROFILE_FUNCTION();
		//auto mesh = kuai::ModelLoader::LoadObj("C:/Users/David/source/repos/smongine/sphere.obj");
		auto mesh = kuai::ModelLoader::loadObj("C:/Users/David/Documents/bunny.obj");

		//kuai::Mesh* mesh = new kuai::Mesh(quadPositions, quadNormals, quadTexCoords, quadIndices);
		//kuai::Texture* texture = new kuai::Texture("C:/Users/David/Pictures/billy.png");
		auto texture = std::make_shared<kuai::Texture>();
	
		auto material = std::make_shared<kuai::PhongMaterial>(texture, texture, 32);
		//auto material = std::make_shared<kuai::SimpleMaterial>();

		auto audio = std::make_shared<kuai::AudioClip>("C:/Users/David/Music/htdc.wav");

		light = scene->createEntity();
		light->addComponent<kuai::Light>();
		light->getComponent<kuai::Light>().setType(kuai::Light::LightType::Directional);
		light->getComponent<kuai::Light>().setIntensity(0.2f);
		light->getTransform().setPos(0, 1, -1);
		light->getTransform().setRot(60, 90, 0);

		cube = scene->createEntity();
		cube->addComponent<kuai::MeshMaterial>(mesh, material);
		cube->getTransform().translate(0, -0.6f, -5);

		cube->addComponent<kuai::AudioSource>();
		auto a = cube->getComponent<kuai::AudioSource>();

		for (int i = 0; i < 3; i++)
		{
			entities.push_back(scene->createEntity());
			int last = entities.size() - 1;
			entities[last]->addComponent<kuai::MeshMaterial>(mesh, material);
			entities[last]->getTransform().translate(rand() % 10 - 5, rand() % 10 - 5, -rand() % 20);
		}


		a.setAudioClip(audio);
		a.setPitch(1.0f);
		a.setLoop(false);
		a.play();
	}

	void update(float dt) override
	{
		KU_PROFILE_FUNCTION();
		scene->update(dt);

		if (kuai::Input::isKeyPressed(kuai::Key::A))
		{
			scene->getMainCam().getTransform().translate(-0.1f, 0, 0);
		}
		if (kuai::Input::isKeyPressed(kuai::Key::D))
		{
			scene->getMainCam().getTransform().translate(0.1f, 0, 0);
		}
		if (kuai::Input::isKeyPressed(kuai::Key::W))
		{
			scene->getMainCam().getTransform().translate(0, 0, -0.1f);
		}
		if (kuai::Input::isKeyPressed(kuai::Key::S))
		{
			scene->getMainCam().getTransform().translate(0, 0, 0.1f);
		}
		if (kuai::Input::isKeyPressed(kuai::Key::Space))
		{
			scene->getMainCam().getTransform().translate(0, 0.1f, 0);
		}
		if (kuai::Input::isKeyPressed(kuai::Key::LeftShift))
		{
			scene->getMainCam().getTransform().translate(0, -0.1f, 0);
		}

		//cube->getTransform().translate(0, 0, -0.05);
	}

	void onEvent(kuai::Event* event) override
	{

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
