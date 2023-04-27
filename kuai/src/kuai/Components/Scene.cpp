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
			500.0f
		);
		mainCam->addComponent<AudioListener>();
		cameraSys->setMainCam(mainCam->getComponent<CameraComponent>());

		mainLight = createEntity();
		mainLight->addComponent<Light>().setType(Light::LightType::Directional);
		//mainLight->getComponent<Light>().setShadows(true);
		mainLight->getComponent<Light>().setIntensity(0.5f);
		mainLight->getTransform().setRot(-45.0f, -140.0f, 0.0f);
	}

	Scene::~Scene()
	{
		delete ECS;
	}

	Rc<Entity> Scene::createEntity()
	{
		KU_PROFILE_FUNCTION();
		Rc<Entity> entity = MakeRc<Entity>(ECS);
		entities.push_back(entity);
		return entity;
	}

	Rc<Entity> Scene::getEntityById(EntityID entity)
	{
		KU_PROFILE_FUNCTION();
		return MakeRc<Entity>(ECS, entity);
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
		cameraSys->update(dt);
		lightSys->update(dt);
	}

	void RenderSystem::insertEntity(EntityID entity)
	{
		System::insertEntity(entity);

		MeshRenderer& mRenderer = scene->getEntityById(entity)->getComponent<MeshRenderer>();

		for (size_t i = 0; i < mRenderer.getModel()->getMeshes().size(); i++)
		{
			Rc<Mesh> mesh = mRenderer.getModel()->getMeshes()[i];
			Shader* shader = mRenderer.getModel()->getMaterials()[i]->getShader();

			IndirectCommand cmd = shaderToCommandMap[shader][mesh->getId()];

			if (meshToInstancesMap[mesh->getId()]++ == 0)
			{
				std::vector<Vertex>& vertexData = shaderToVertexDataMap[shader];
				std::vector<u32>& indices = shaderToIndicesMap[shader];

				cmd.count = mesh->indices.size();
				cmd.firstIndex = vertexData.size();
				cmd.baseVertex = indices.size();
				cmd.baseInstance = totalInstances;

				shaderToVertexDataSizesMap[shader][mesh->getId()] = mesh->vertexData.size();
				shaderToIndicesSizesMap[shader][mesh->getId()] = mesh->indices.size();

				vertexData.insert(vertexData.end(), mesh->vertexData.begin(), mesh->vertexData.end());
				indices.insert(indices.end(), mesh->indices.begin(), mesh->indices.end());

				dataChanged = true;
			}

			std::vector<glm::mat4>& modelMatrices = shaderToModelMatrixMap[shader];
			modelMatrices.push_back(mRenderer.getTransform().getModelMatrix());

			cmd.instanceCount = meshToInstancesMap[mesh->getId()];
			totalInstances++;

			shaderToCommandMap[shader][mesh->getId()] = cmd;
		}

		std::vector<IndirectCommand> commands;
		for (auto& pair : shaderToCommandMap)
		{
			for (auto& innerPair : pair.second)
			{
				commands.push_back(innerPair.second);
			}
			pair.first->setIndirectBufData(commands);
			commands.clear();
		}	

		//Renderer::updateShadowMap(scene->getMainLight());
	}

	void RenderSystem::removeEntity(EntityID entity)
	{
		MeshRenderer& mRenderer = scene->getEntityById(entity)->getComponent<MeshRenderer>();

		for (size_t i = 0; i < mRenderer.getModel()->getMeshes().size(); i++)
		{
			Rc<Mesh> mesh = mRenderer.getModel()->getMeshes()[i];
			Shader* shader = mRenderer.getModel()->getMaterials()[i]->getShader();

			IndirectCommand cmd = shaderToCommandMap[shader][mesh->getId()];

			cmd.instanceCount = --meshToInstancesMap[mesh->getId()];

			std::vector<glm::mat4>& modelMatrices = shaderToModelMatrixMap[shader];
			modelMatrices.erase(modelMatrices.begin() + cmd.baseInstance + cmd.instanceCount);

			totalInstances--;
			dataChanged = true;

			if (totalInstances == 0)
			{
				std::vector<Vertex>& vertexData = shaderToVertexDataMap[shader];
				std::vector<u32>& indices = shaderToIndicesMap[shader];

				u32 vertDataSize = shaderToVertexDataSizesMap[shader][mesh->getId()];
				u32 indicesSize = shaderToIndicesSizesMap[shader][mesh->getId()];

				vertexData.erase(vertexData.begin() + cmd.firstIndex, vertexData.begin() + cmd.firstIndex + vertDataSize);
				indices.erase(indices.begin() + cmd.baseVertex, indices.begin() + cmd.baseVertex + indicesSize);

				shaderToCommandMap[shader].erase(mesh->getId());
			}
			else
				shaderToCommandMap[shader][mesh->getId()] = cmd;

			System::removeEntity(entity);
		}
	}

	void RenderSystem::update(float dt)
	{

	}

	void RenderSystem::render()
	{
		KU_PROFILE_FUNCTION();

		if (dataChanged)
		{
			for (auto& pair : shaderToVertexDataMap)
			{
				pair.first->getVertexArray()->getVertexBuffers()[0]->reset(pair.second.data(), pair.second.size() * sizeof(Vertex));
			}

			for (auto& pair : shaderToIndicesMap)
			{
				pair.first->getVertexArray()->setIndexBuffer(MakeRc<IndexBuffer>(pair.second.data(), pair.second.size()));
			}

			for (auto& pair : shaderToModelMatrixMap)
			{
				pair.first->getVertexArray()->getVertexBuffers()[1]->reset(pair.second.data(), pair.second.size() * sizeof(glm::mat4), DrawHint::DYNAMIC);
			}
			dataChanged = false;
		}
		else
		{
			for (auto& pair : shaderToModelMatrixMap)
			{
				pair.first->getVertexArray()->getVertexBuffers()[1]->setData(pair.second.data(), pair.second.size() * sizeof(glm::mat4));
			}
		}

		// Renderer::updateShadowMap(scene->getMainLight());

		Renderer::setViewport(0, 0, App::get().getWindow().getWidth(), App::get().getWindow().getHeight());
		Renderer::clear();

		// TODO: have some central list of shaders that the renderer cycles through
		Renderer::render(*StaticShader::skybox);
		Renderer::render(*StaticShader::basic);
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
			CameraComponent& c = entity->getComponent<CameraComponent>();

			if (c.changed)
			{
				// CameraChangedEvent e(&c);
				// scene->notifySystems(&e);

				// Projection and view matrices are stored in global shader memory so any shader that references them can update the memory
				StaticShader::basic->bind();

				StaticShader::basic->setUniform("CamData", "projectionMatrix", &c.getProjectionMatrix()[0][0], sizeof(glm::mat4));
				StaticShader::basic->setUniform("CamData", "viewMatrix", &c.getViewMatrix()[0][0], sizeof(glm::mat4));
				StaticShader::basic->setUniform("CamData", "viewPos", &c.getTransform().getPos()[0], sizeof(glm::vec3));

				c.changed = false;
			}
			if (c.getTarget())
			{
				c.getTarget()->bind();
				renderSys->render();
				c.getTarget()->unbind();
			}
			else if (c.isMain)
			{
				renderSys->render();
			}
		}
	}

	void LightSystem::update(float dt)
	{
		for (auto& entity : entities)
		{
			Light& l = entity->getComponent<Light>();
			if (l.changed)
			{
				// LightChangedEvent e(&l);
				// scene->notifySystems(&e);

				int id = l.getId();
				int type = (int)l.getType();
				float intensity = l.getIntensity();
				float linear = l.getLinear();
				float quadratic = l.getQuadratic();
				float cutoff = glm::cos(glm::radians(l.getAngle()));
				int castsShadows = l.castsShadows();

				StaticShader::basic->setUniform("Lights", "lights[" + std::to_string(id) + "].type", &type, sizeof(int));

				StaticShader::basic->setUniform("Lights", "lights[" + std::to_string(id) + "].pos", &l.getTransform().getPos()[0], sizeof(glm::vec3));
				StaticShader::basic->setUniform("Lights", "lights[" + std::to_string(id) + "].dir", &l.getTransform().getForward()[0], sizeof(glm::vec3));
				StaticShader::basic->setUniform("Lights", "lights[" + std::to_string(id) + "].col", &l.getCol()[0], sizeof(glm::vec3));

				StaticShader::basic->setUniform("Lights", "lights[" + std::to_string(id) + "].intensity", &intensity, sizeof(float));
				StaticShader::basic->setUniform("Lights", "lights[" + std::to_string(id) + "].linear", &linear, sizeof(float));
				StaticShader::basic->setUniform("Lights", "lights[" + std::to_string(id) + "].quadratic", &quadratic, sizeof(float));
				StaticShader::basic->setUniform("Lights", "lights[" + std::to_string(id) + "].cutoff", &cutoff, sizeof(float));

				StaticShader::basic->setUniform("Lights", "lights[" + std::to_string(id) + "].castsShadows", &castsShadows, sizeof(int));

				l.changed = false;
			}
		}
	}
}
