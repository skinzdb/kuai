#include "smpch.h"
#include "Scene.h"

namespace Smong {
	Scene::Scene()
	{
		ECS = std::make_shared<EntityComponentSystem>();
		ECS->Init();

		ECS->RegisterComponent<Transform>();
		ECS->RegisterComponent<Camera>();

		mainCam = Camera();
	}

	Scene::~Scene()
	{
		for (auto& pair : entityMap)
		{
			delete pair.second;
		}
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

	void Scene::GetLights()
	{
	
	}

	void Scene::Update(float dt)
	{
	}



}
