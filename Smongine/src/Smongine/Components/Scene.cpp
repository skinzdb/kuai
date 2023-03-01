#include "smpch.h"
#include "Scene.h"

#include "Smongine/Renderer/Renderer.h"
#include "Smongine/Core/App.h"

namespace Smong {
	Scene::Scene()
	{
		ECS = new EntityComponentSystem();

		ECS->RegisterComponent<Transform>();
		ECS->RegisterComponent<Camera>();
		ECS->RegisterComponent<Light>();
		ECS->RegisterComponent<MeshMaterial>();

		this->lightSys = ECS->RegisterSystem<LightSystem>(this);
		ECS->SetSystemMask<LightSystem>(BIT(ECS->GetComponentType<Light>()));

		this->renderSys = ECS->RegisterSystem<RenderSystem>(this);
		ECS->SetSystemMask<RenderSystem>(BIT(ECS->GetComponentType<MeshMaterial>()));
		
		//this->cameraSys = ECS->RegisterSystem<CameraSystem>(this);
		//ECS->SetSystemMask<CameraSystem>(BIT(ECS->GetComponentType<Camera>()));

		mainCam = CreateEntity();
		mainCam->AddComponent<Camera>(
			70.0f, 
			(float)App::Get().GetWindow().GetWidth(),
			(float)App::Get().GetWindow().GetHeight(),
			0.1f,
			100.0f
		);
	}

	Scene::~Scene()
	{
		delete ECS;
	}

	std::shared_ptr<Entity> Scene::CreateEntity()
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>(ECS);
		entities.push_back(entity);
		return entity;
	}

	std::shared_ptr<Entity> Scene::GetEntityById(EntityID entity)
	{
		return std::make_shared<Entity>(ECS, entity);
	}

	void Scene::DestroyEntity(EntityID entity)
	{
		ECS->DestroyEntity(entity);
	}

	Camera& Scene::GetMainCam()
	{
		return mainCam->GetComponent<Camera>();
	}

	void Scene::SetMainCam(Camera& cam)
	{
		this->mainCam->GetComponent<Camera>() = cam;
	}

	void Scene::Update(float dt)
	{
		lightSys->Update(dt);
		renderSys->Update(dt);
	}

	void RenderSystem::Update(float dt)
	{
		Renderer::Clear();
		for (auto& entity : entities)
		{
			Renderer::Render(entity->GetComponent<MeshMaterial>(), entity->GetTransform().GetModelMatrix());
		}
	}

	void LightSystem::Update(float dt)
	{
		Renderer::SetLights(entities);
	}

	//void CameraSystem::Update(float dt)
	//{
	//	for (auto& entity : entities)
	//	{
	//		Renderer::SetCamera(entity->GetComponent<Camera>(), entity->GetTransform().GetPos());
	//	}
	//}

}
