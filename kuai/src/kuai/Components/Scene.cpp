#include "kpch.h"
#include "Scene.h"

#include "kuai/Renderer/Renderer.h"
#include "kuai/Core/App.h"

namespace kuai {
	Scene::Scene()
	{
		ECS = new EntityComponentSystem();

		ECS->registerComponent<Transform>();
		ECS->registerComponent<Camera>();
		ECS->registerComponent<Light>();
		ECS->registerComponent<MeshMaterial>();

		this->lightSys = ECS->registerSystem<LightSystem>(this);
		ECS->setSystemMask<LightSystem>(BIT(ECS->getComponentType<Light>()));

		this->renderSys = ECS->registerSystem<RenderSystem>(this);
		ECS->setSystemMask<RenderSystem>(BIT(ECS->getComponentType<MeshMaterial>()));
		
		//this->cameraSys = ECS->RegisterSystem<CameraSystem>(this);
		//ECS->setSystemMask<CameraSystem>(BIT(ECS->getComponentType<Camera>()));

		mainCam = createEntity();
		mainCam->addComponent<Camera>(
			70.0f, 
			(float)App::get().getWindow().getWidth(),
			(float)App::get().getWindow().getHeight(),
			0.1f,
			100.0f
		);
	}

	Scene::~Scene()
	{
		delete ECS;
	}

	std::shared_ptr<Entity> Scene::createEntity()
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>(ECS);
		entities.push_back(entity);
		return entity;
	}

	std::shared_ptr<Entity> Scene::getEntityById(EntityID entity)
	{
		return std::make_shared<Entity>(ECS, entity);
	}

	void Scene::destroyEntity(EntityID entity)
	{
		ECS->destroyEntity(entity);
	}

	Camera& Scene::getMainCam()
	{
		return mainCam->getComponent<Camera>();
	}

	void Scene::setMainCam(Camera& cam)
	{
		this->mainCam->getComponent<Camera>() = cam;
	}

	void Scene::update(float dt)
	{
		lightSys->update(dt);
		renderSys->update(dt);
	}

	void RenderSystem::update(float dt)
	{
		Renderer::clear();
		for (auto& entity : entities)
		{
			Renderer::render(entity->getComponent<MeshMaterial>(), entity->getTransform().getModelMatrix());
		}
	}

	void LightSystem::update(float dt)
	{
		Renderer::setLights(entities);
	}

	//void CameraSystem::update(float dt)
	//{
	//	for (auto& entity : entities)
	//	{
	//		Renderer::setCamera(entity->getComponent<Camera>(), entity->getTransform().getPos());
	//	}
	//}

}
