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

		this->lightSystem = ECS->RegisterSystem<LightSystem>();
		ECS->SetSystemMask<LightSystem>(BIT(ECS->GetComponentType<Light>()));

		/*ECS->RegisterSystem<CameraTransformSystem>();
		ECS->SetSystemMask<CameraTransformSystem>(BIT(ECS->GetComponentType<Camera>()) | BIT(ECS->GetComponentType<Transform>()));*/

		mainCam = Camera(
			70.0f, 
			(float)App::Get().GetWindow().GetWidth(),
			(float)App::Get().GetWindow().GetHeight(),
			0.1f,
			100.0f
		);
	}

	Scene::~Scene()
	{
		for (auto& pair : entityMap)
		{
			delete pair.second;
		}
		delete ECS;
	}

	Entity* Scene::CreateEntity()
	{
		Entity* entity = new Entity(ECS);
		entityMap.insert({ entity->GetId(), entity });
		return entity;
	}

	Entity* Scene::GetEntityById(EntityID entity)
	{
		auto pos = entityMap.find(entity);

		SM_CORE_ASSERT(pos != entityMap.end(), "Entity with ID={0} does not exist", entity);

		return pos->second;
	}

	void Scene::DestroyEntity(EntityID entity)
	{
		ECS->DestroyEntity(entity);
	}

	Camera Scene::GetMainCam()
	{
		return mainCam;
	}

	void Scene::SetMainCam(Camera cam)
	{
		this->mainCam = cam;
	}

	std::vector<EntityID> Scene::GetLights()
	{
		return lightSystem->GetEntities();
	}

	void Scene::Update(float dt)
	{
		Renderer::Render(*this);
	}



}
