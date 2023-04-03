#include "kpch.h"
#include "Scene.h"

#include "kuai/Renderer/Renderer.h"
#include "kuai/Sound/AudioManager.h"
#include "kuai/Core/App.h"

#include "kuai/Renderer/StaticShader.h"

namespace kuai {
	Scene::Scene()
	{
		ECS = new EntityComponentSystem();

		ECS->registerComponent<Transform>();
		ECS->registerComponent<Camera>();
		ECS->registerComponent<Light>();
		ECS->registerComponent<MeshRenderer>();
		ECS->registerComponent<AudioListener>();
		ECS->registerComponent<AudioSource>();

		this->renderSys = ECS->registerSystem<RenderSystem>(this);
		ECS->setSystemMask<RenderSystem>(BIT(ECS->getComponentType<MeshRenderer>()));

		this->cameraSys = ECS->registerSystem<CameraSystem>(this);
		ECS->setSystemMask<CameraSystem>(BIT(ECS->getComponentType<Camera>()));

		this->lightSys = ECS->registerSystem<LightSystem>(this);
		ECS->setSystemMask<LightSystem>(BIT(ECS->getComponentType<Light>()));

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

		mainLight = createEntity();
		mainLight->addComponent<Light>(0.1f);
		mainLight->getTransform().setRot(-60.0f, -60.0f, 0.0f);
	}

	Scene::~Scene()
	{
		delete ECS;
	}

	std::shared_ptr<Entity> Scene::createEntity()
	{
		KU_PROFILE_FUNCTION();
		std::shared_ptr<Entity> entity = std::make_shared<Entity>(ECS);
		entities.push_back(entity);
		return entity;
	}

	std::shared_ptr<Entity> Scene::getEntityById(EntityID entity)
	{
		KU_PROFILE_FUNCTION();
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

	Light& Scene::getMainLight()
	{
		return mainLight->getComponent<Light>();
	}

	void Scene::setMainLight(Light& light)
	{
		this->mainLight->getComponent<Light>() = light;
	}

	void Scene::update(float dt)
	{
		lightSys->update(dt);
		cameraSys->update(dt);
		renderSys->update(dt);
		audioSys->update(dt);
	}

	RenderSystem::RenderSystem(Scene* scene) : System(scene)
	{
		acceptsSubset = true;
		scene->subscribeSystem(this, &RenderSystem::onLightChanged);
		scene->subscribeSystem(this, &RenderSystem::onCameraChanged);
	}

	void RenderSystem::update(float dt)
	{
		KU_PROFILE_FUNCTION();

		Renderer::clear();
		for (auto& entity : entities)
		{
			MeshRenderer m = entity->getComponent<MeshRenderer>();
			for (auto& mesh : m.getModel()->getMeshes())
			{
				mesh->getMaterial()->getShader()->getData()->modelMatrix = entity->getTransform().getModelMatrix();
				mesh->getMaterial()->getShader()->updateTransform();

				Renderer::render(*mesh);
			}
		}
	}

	void RenderSystem::onLightChanged(LightChangedEvent* event)
	{
		for (auto& entity : entities)
		{
			MeshRenderer m = entity->getComponent<MeshRenderer>();
			for (auto& mesh : m.getModel()->getMeshes())
			{
				mesh->getMaterial()->getShader()->getData()->light = event->light;
				mesh->getMaterial()->getShader()->updateLight();
			}
		}
	}

	void RenderSystem::onCameraChanged(CameraChangedEvent* event)
	{
		for (auto& entity : entities)
		{
			MeshRenderer m = entity->getComponent<MeshRenderer>();
			for (auto& mesh : m.getModel()->getMeshes())
			{
				mesh->getMaterial()->getShader()->getData()->projectionMatrix = event->cam->getProjectionMatrix();
				mesh->getMaterial()->getShader()->getData()->viewMatrix = event->cam->getViewMatrix();

				mesh->getMaterial()->getShader()->updateCamera();
			}
		}
	}

	void CameraSystem::update(float dt)
	{
		for (auto& entity : entities)
		{
			Camera c = entity->getComponent<Camera>();
			if (c.changed)
			{
				CameraChangedEvent e(&c);
				scene->notifySystems(&e);
				c.changed = false;
			}
		}
	}

	void LightSystem::update(float dt)
	{
		for (auto& entity : entities)
		{
			Light l = entity->getComponent<Light>();
			if (l.changed)
			{
				LightChangedEvent e(&l);
				scene->notifySystems(&e);
				l.changed = false;
			}
		}
	}

	void AudioSystem::update(float dt)
	{
		for (auto& entity : entities)
		{
			AudioManager::updateStream(entity->getComponent<AudioSource>().getId());
		}
	}
}
