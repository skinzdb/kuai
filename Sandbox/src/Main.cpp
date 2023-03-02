#include <kuai.h>

#include <glm/gtx/string_cast.hpp>

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

	BaseLayer() : Layer("Base"), scene(std::make_unique<kuai::Scene>())
	{
		//auto mesh = kuai::ModelLoader::LoadObj("C:/Users/David/source/repos/smongine/sphere.obj");
		auto mesh = kuai::ModelLoader::LoadObj("C:/Users/David/Documents/FinalBaseMesh.obj");

		//kuai::Mesh* mesh = new kuai::Mesh(quadPositions, quadNormals, quadTexCoords, quadIndices);
		//kuai::Texture* texture = new kuai::Texture("C:/Users/David/Pictures/billy.png");
		auto texture = std::make_shared<kuai::Texture>();
	
		auto material = std::make_shared<kuai::PhongMaterial>(texture, texture, 32);
		//auto material = std::make_shared<kuai::SimpleMaterial>();

		light = scene->createEntity();
		light->addComponent<kuai::Light>();
		light->getTransform().setPos(0, 1, -1);
		light->getTransform().setRot(0, 0, 0);

		cube = scene->createEntity();
		cube->addComponent<kuai::MeshMaterial>(mesh, material);
		cube->getTransform().translate(0, -0.6f, -5);
	}

	void update(float dt) override
	{
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

		cube->getTransform().rotate(0, 0.01f, 0);
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
	KU_CORE_WARN("Initialised Core Log");
	KU_CORE_INFO("Initialised Client Log");

	kuai::App* app = new kuai::App();
	app->pushLayer(new BaseLayer());
	app->run();
	delete app;

	return 0;
}
