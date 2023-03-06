#include "kpch.h"
#include "Scene.h"

#include "kuai/Renderer/Renderer.h"
#include "kuai/Sound/AudioManager.h"
#include "kuai/Core/App.h"

namespace kuai {
	Scene::Scene()
	{
		ECS = new EntityComponentSystem();

		ECS->registerComponent<Transform>();
		ECS->registerComponent<Camera>();
		ECS->registerComponent<Light>();
		ECS->registerComponent<MeshMaterial>();
		ECS->registerComponent<AudioListener>();
		ECS->registerComponent<AudioSource>();

		this->lightSys = ECS->registerSystem<LightSystem>(this);
		ECS->setSystemMask<LightSystem>(BIT(ECS->getComponentType<Light>()));

		this->renderSys = ECS->registerSystem<RenderSystem>(this);
		ECS->setSystemMask<RenderSystem>(BIT(ECS->getComponentType<MeshMaterial>()));
		
		this->audioSys = ECS->registerSystem<AudioSystem>(this);
		ECS->setSystemMask<AudioSystem>(BIT(ECS->getComponentType<AudioSource>()));

		mainCam = createEntity();
		mainCam->addComponent<Camera>(
			70.0f, 
			(float)App::get().getWindow().getWidth(),
			(float)App::get().getWindow().getHeight(),
			0.1f,
			100.0f
		);
		mainCam->addComponent<AudioListener>();
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
		audioSys->update(dt);
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

	void AudioSystem::update(float dt)
	{
		for (auto& entity : entities)
		{
			AudioManager::updateStream(entity->getComponent<AudioSource>().getId());
		}
	}
}
