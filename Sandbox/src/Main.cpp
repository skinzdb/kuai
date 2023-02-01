#include <Smongine.h>

#include <glm/gtx/string_cast.hpp>

class BaseLayer : public Smong::Layer
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

	std::shared_ptr<Smong::Entity> cube;
	std::shared_ptr<Smong::Entity> light;

	BaseLayer() : Layer("Base"), scene(std::make_unique<Smong::Scene>())
	{
		Smong::Mesh* mesh = Smong::ModelLoader::LoadObj("C:/Users/David/source/repos/smongine/bunny.obj");
		//Smong::Mesh* mesh = Smong::ModelLoader::LoadObj("C:/Users/David/Documents/FinalBaseMesh.obj");

		//Smong::Mesh* mesh = new Smong::Mesh(quadPositions, quadNormals, quadTexCoords, quadIndices);
		//Smong::Texture* texture = new Smong::Texture("C:/Users/David/Pictures/billy.png");
		Smong::Texture* texture = new Smong::Texture();
	
		Smong::PhongMaterial* material = new Smong::PhongMaterial(*texture, *texture, 32);
		//Smong::SimpleMaterial* material = new Smong::SimpleMaterial();

		light = scene->CreateEntity();
		light->AddComponent<Smong::Light>(1.0f, 0.5f, 0.05f);
		light->GetTransform().pos = glm::vec3(0, 1, -1);
		light->GetTransform().rot = glm::vec3(0, 0, 0);

		cube = scene->CreateEntity();
		cube->AddComponent<Smong::MeshMaterial>(*mesh, *material);
		cube->GetTransform().pos += glm::vec3(0, -0.6f, -3);
	}

	void Update(float dt) override
	{
		scene->Update(dt);

		if (Smong::Input::IsKeyPressed(Smong::Key::A))
		{
			scene->GetMainCamTransform().pos += glm::vec3(-0.1f, 0, 0);
		}
		if (Smong::Input::IsKeyPressed(Smong::Key::D))
		{
			scene->GetMainCamTransform().pos += glm::vec3(0.1f, 0, 0);
		}
		if (Smong::Input::IsKeyPressed(Smong::Key::W))
		{
			scene->GetMainCamTransform().pos += glm::vec3(0, 0, -0.1f);
		}
		if (Smong::Input::IsKeyPressed(Smong::Key::S))
		{
			scene->GetMainCamTransform().pos += glm::vec3(0, 0, 0.1f);
		}
		if (Smong::Input::IsKeyPressed(Smong::Key::Space))
		{
			scene->GetMainCamTransform().pos += glm::vec3(0, 0.1f, 0);
		}
		if (Smong::Input::IsKeyPressed(Smong::Key::LeftShift))
		{
			scene->GetMainCamTransform().pos += glm::vec3(0, -0.1f, 0);
		}
	}

	void OnEvent(Smong::Event& event) override
	{

	}

private:
	std::unique_ptr<Smong::Scene> scene;
};

class Sandbox : public Smong::App {
public:
	Sandbox() {
		PushLayer(new BaseLayer());
	}

	~Sandbox() {

	}
};

Smong::App* Smong::CreateApp()
{
	return new Sandbox();
}

