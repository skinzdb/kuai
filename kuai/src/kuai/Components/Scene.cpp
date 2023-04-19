#include "kpch.h"
#include "Scene.h"

#include "kuai/Renderer/Renderer.h"
#include "kuai/Core/App.h"

#include "kuai/Renderer/StaticShader.h"

namespace kuai {
	Scene::Scene()
	{
		ECS = new EntityComponentSystem();

		ECS->registerComponent<Transform>();
		ECS->registerComponent<CameraComponent>();
		ECS->registerComponent<Light>();
		ECS->registerComponent<MeshRenderer>();
		ECS->registerComponent<AudioListener>();
		ECS->registerComponent<AudioSourceComponent>();

		renderSys = ECS->registerSystem<RenderSystem>(this);
		ECS->setSystemMask<RenderSystem>(BIT(ECS->getComponentType<MeshRenderer>()));

		lightSys = ECS->registerSystem<LightSystem>(this);
		ECS->setSystemMask<LightSystem>(BIT(ECS->getComponentType<Light>()));
		lightSys->setRenderSystem(renderSys);

		cameraSys = ECS->registerSystem<CameraSystem>(this);
		ECS->setSystemMask<CameraSystem>(BIT(ECS->getComponentType<CameraComponent>()));
		cameraSys->setRenderSystem(renderSys);

		mainCam = createEntity();
		mainCam->addComponent<CameraComponent>(
			70.0f, 
			(float)App::get().getWindow().getWidth(),
			(float)App::get().getWindow().getHeight(),
			0.1f,
			100.0f
		);
		mainCam->addComponent<AudioListener>();
		cameraSys->setMainCam(mainCam->getComponent<CameraComponent>());

		mainLight = createEntity();
		mainLight->addComponent<Light>().setType(Light::LightType::Directional);
		mainLight->getComponent<Light>().setShadows(true);
		mainLight->getComponent<Light>().setIntensity(0.5f);
		mainLight->getTransform().setRot(-45.0f, -140.0f, 0.0f);
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

	CameraComponent& Scene::getMainCam()
	{
		return mainCam->getComponent<CameraComponent>();
	}

	void Scene::setMainCam(CameraComponent& cam)
	{
		this->mainCam->getComponent<CameraComponent>() = cam;
		cameraSys->setMainCam(cam);
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

		Renderer::updateShadowMap(scene->getMainLight()); // TODO: put in a function that gets called when objects change within light's range or light moves...

		Renderer::setViewport(0, 0, App::get().getWindow().getWidth(), App::get().getWindow().getHeight());
		Renderer::clear();

		Renderer::setMeshes(entities);
		Renderer::render();
	}

	void RenderSystem::onLightChanged(LightChangedEvent* event)
	{
		for (auto& entity : entities)
		{
			MeshRenderer m = entity->getComponent<MeshRenderer>();
			for (auto& mesh : m.getModel()->getMeshes())
			{
				Shader* shader = mesh->getMaterial()->getShader();
				if (shader != StaticShader::basic)
					continue;

				Light* l = event->light;

				shader->bind();

				// shader->setUniform("Lights", "lights[" + std::to_string(l->getId()) + "].type", &type, sizeof(int));

				// shader->setUniform("Lights", "lights[" + std::to_string(l->getId()) + "].pos", &l->getTransform().getPos()[0], sizeof(glm::vec3));
				// shader->setUniform("Lights", "lights[" + std::to_string(l->getId()) + "].dir", &l->getTransform().getForward()[0], sizeof(glm::vec3));
				// shader->setUniform("Lights", "lights[" + std::to_string(l->getId()) + "].col", &l->getCol()[0], sizeof(glm::vec3));

				// shader->setUniform("Lights", "lights[" + std::to_string(l->getId()) + "].intensity", &intensity, sizeof(float));
				// shader->setUniform("Lights", "lights[" + std::to_string(l->getId()) + "].linear", &linear, sizeof(float));
				// shader->setUniform("Lights", "lights[" + std::to_string(l->getId()) + "].quadratic", &quadratic, sizeof(float));
				// shader->setUniform("Lights", "lights[" + std::to_string(l->getId()) + "].cutoff", &cutoff, sizeof(float));

				shader->setUniform("lights[" + std::to_string(l->getId()) + "].type",  (int)l->getType());

				shader->setUniform("lights[" + std::to_string(l->getId()) + "].pos", l->getTransform().getPos());
				shader->setUniform("lights[" + std::to_string(l->getId()) + "].dir", l->getTransform().getForward());
				shader->setUniform("lights[" + std::to_string(l->getId()) + "].col", l->getCol());

				shader->setUniform("lights[" + std::to_string(l->getId()) + "].intensity", l->getIntensity());
				shader->setUniform("lights[" + std::to_string(l->getId()) + "].linear", l->getLinear());
				shader->setUniform("lights[" + std::to_string(l->getId()) + "].quadratic", l->getQuadratic());
				shader->setUniform("lights[" + std::to_string(l->getId()) + "].cutoff", glm::cos(glm::radians(l->getAngle())));

				shader->setUniform("lights[" + std::to_string(l->getId()) + "].castShadows", l->castsShadows());

				if (l->castsShadows())
					shader->setUniform("lightSpaceMatrix", l->getLightSpaceMatrix()); // TODO: lightSpaceMatrix per light 
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
				Shader* shader = mesh->getMaterial()->getShader();
				CameraComponent* c = event->cam;

				shader->bind();

				shader->setUniform("Matrices", "projectionMatrix", &c->cam.getProjectionMatrix()[0][0], sizeof(glm::mat4));
				shader->setUniform("Matrices", "viewMatrix", &c->cam.getViewMatrix()[0][0], sizeof(glm::mat4));

				if (shader == StaticShader::basic)
					shader->setUniform("viewPos", c->getTransform().getPos());
			}
		}
	}

	void CameraSystem::setMainCam(CameraComponent& cam)
	{
		for (auto& entity : entities) 
			entity->getComponent<CameraComponent>().isMain = false;

		cam.isMain = true;
	}

	void CameraSystem::update(float dt)
	{
		for (auto& entity : entities)
		{
			CameraComponent c = entity->getComponent<CameraComponent>();

			if (c.changed)
			{
				CameraChangedEvent e(&c);
				scene->notifySystems(&e);
				c.changed = false;
			}
			if (c.cam.getTarget())
			{
				c.cam.getTarget()->bind();
				renderSys->update(dt);
				c.cam.getTarget()->unbind();
			}
			else if (c.isMain)
			{
				renderSys->update(dt);
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
}
