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
			90.0f, 
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
		entityMap[entity->getId()] = entity;

		return entity;
	}

	Rc<Entity> Scene::getEntityById(EntityID entity)
	{
		KU_PROFILE_FUNCTION();
		return entityMap[entity];
	}

	void Scene::destroyEntity(EntityID entity)
	{
		ECS->destroyEntity(entity);
		entityMap.erase(entity);
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
		renderSys->update(dt);
		cameraSys->update(dt);
		lightSys->update(dt);
	}

	void RenderSystem::insertEntity(EntityID entity)
	{
		System::insertEntity(entity);

		MeshRenderer& mRenderer = scene->getEntityById(entity)->getComponent<MeshRenderer>();

		// For every mesh in the model
		for (size_t i = 0; i < mRenderer.getModel()->getMeshes().size(); i++)
		{
			Rc<Mesh> mesh = mRenderer.getModel()->getMeshes()[i];
			u32 id = mesh->getId();
			Shader* shader = mRenderer.getModel()->getMaterials()[i]->getShader();

			IndirectCommand& cmd = shaderToMeshCommand[shader][id];
			cmd.instanceCount = ++meshToNumInstances[id]; // Increment instance count by one

			if (cmd.instanceCount == 1) // First instance, so add its vertex data and indices to back of the list
			{
				std::vector<Vertex>& vertexData = shaderToVertexData[shader];
				std::vector<u32>& indices = shaderToIndices[shader];
				// Setup this mesh's render command
				cmd.count = mesh->indices.size();				// Number of indices mesh uses
				cmd.firstIndex = indices.size();				// Offset of first index
				cmd.baseVertex = vertexData.size();				// Offset of first vertex
				cmd.baseInstance = shaderToInstances[shader];	// Offset of first instance

				shaderToVertexDataSizes[shader][id] = mesh->vertexData.size();
				shaderToIndicesSizes[shader][id] = mesh->indices.size();
				// Insert the data at the end
				vertexData.insert(vertexData.end(), mesh->vertexData.begin(), mesh->vertexData.end());
				indices.insert(indices.end(), mesh->indices.begin(), mesh->indices.end());

				dataChanged = true;
			}

			shaderToMeshCommand[shader][id] = cmd;

			shaderToInstances[shader]++;
		}

		// Resize model matrices and set commands
		StaticShader::basic->getVertexArray()->getVertexBuffers()[1]->reset(nullptr, shaderToInstances[StaticShader::basic] * sizeof(glm::mat4), DrawHint::DYNAMIC);
		setCommands();

		//Renderer::updateShadowMap(scene->getMainLight());
	}

	void RenderSystem::removeEntity(EntityID entity)
	{
		MeshRenderer& mRenderer = scene->getEntityById(entity)->getComponent<MeshRenderer>();

		for (size_t i = 0; i < mRenderer.getModel()->getMeshes().size(); i++)
		{
			Rc<Mesh> mesh = mRenderer.getModel()->getMeshes()[i];
			u32 id = mesh->getId();
			Shader* shader = mRenderer.getModel()->getMaterials()[i]->getShader();

			IndirectCommand& cmd = shaderToMeshCommand[shader][id];
			cmd.instanceCount = --meshToNumInstances[id]; // Decrement instance count by one

			if (cmd.instanceCount == 0) // No more instances of this mesh left -> remove its vertex data and indices from lists
			{
				std::vector<Vertex>& vertexData = shaderToVertexData[shader];
				std::vector<u32>& indices = shaderToIndices[shader];

				size_t vertexDataSize = shaderToVertexDataSizes[shader][id];
				size_t indicesSize = shaderToIndicesSizes[shader][id];

				vertexData.erase(vertexData.begin() + cmd.baseVertex, vertexData.begin() + cmd.baseVertex + vertexDataSize);
				indices.erase(indices.begin() + cmd.firstIndex, indices.begin() + cmd.firstIndex + indicesSize);

				// Take away vertex data size and indices size from offset for all meshes that are further along in the list
				for (auto& pair : shaderToMeshCommand)
				{
					for (auto& innerPair : pair.second)
					{
						if (innerPair.second.baseInstance > cmd.baseInstance)
						{
							innerPair.second.baseVertex -= vertexDataSize;
							innerPair.second.firstIndex -= indicesSize;
						}
					}
				}

				shaderToVertexDataSizes[shader].erase(id);
				shaderToIndicesSizes[shader].erase(id);
				meshToNumInstances.erase(id);

				dataChanged = true;
			}
			// Decrement base instances of all meshes that are further along the list as we deleted an instance 
			for (auto& pair : shaderToMeshCommand)
			{
				for (auto& innerPair : pair.second)
				{
					if (innerPair.second.baseInstance > cmd.baseInstance)
						innerPair.second.baseInstance--;
				}
			}

			if (cmd.instanceCount == 0)
				shaderToMeshCommand[shader].erase(id); // Remove the render command

			shaderToInstances[shader]--;
		}

		// Resize model matrices and set commands
		StaticShader::basic->getVertexArray()->getVertexBuffers()[1]->reset(nullptr, shaderToInstances[StaticShader::basic] * sizeof(glm::mat4), DrawHint::DYNAMIC);
		setCommands();

		System::removeEntity(entity);
	}

	void RenderSystem::update(float dt)
	{
		// Update model matrices every frame
		// TODO: inefficient, only update when transform moves

		for (auto& pair : shaderToInstances)
		{
			std::vector<glm::mat4> modelMatrices(pair.second);
			for (size_t i = 0; i < entities.size(); i++)
				modelMatrices[i] = entities[i]->getTransform().getModelMatrix();
			shaderToModelMatrices[StaticShader::basic] = modelMatrices;
		}
	}

	void RenderSystem::render()
	{
		//KU_PROFILE_FUNCTION();

		if (dataChanged)
		{
			for (auto& pair : shaderToVertexData)
				pair.first->getVertexArray()->getVertexBuffers()[0]->reset(pair.second.data(), pair.second.size() * sizeof(Vertex));

			for (auto& pair : shaderToIndices)
				pair.first->getVertexArray()->setIndexBuffer(MakeRc<IndexBuffer>(pair.second.data(), pair.second.size()));

			dataChanged = false;
		}
		else
		{
			for (auto& pair : shaderToModelMatrices)
				pair.first->getVertexArray()->getVertexBuffers()[1]->setData(pair.second.data(), pair.second.size() * sizeof(glm::mat4));
		}

		// Renderer::updateShadowMap(scene->getMainLight());

		Renderer::setViewport(0, 0, App::get().getWindow().getWidth(), App::get().getWindow().getHeight());
		Renderer::clear();

		// TODO: have some central list of shaders that the renderer cycles through
		// Renderer::render(*StaticShader::skybox);
		Renderer::render(*StaticShader::basic);
	}
	
	void RenderSystem::setCommands()
	{
		std::vector<IndirectCommand> commands;
		for (auto& pair : shaderToMeshCommand)
		{
			for (auto& innerPair : pair.second)
				commands.push_back(innerPair.second);
			
			pair.first->setIndirectBufData(commands);
			commands.clear();
		}
		// KU_CORE_ERROR("*** count:{0}, no. instances:{1}, vert offset:{2}, index offset:{3}, base instance:{4} ***", innerPair.second.count, innerPair.second.instanceCount, innerPair.second.firstIndex, innerPair.second.baseVertex, innerPair.second.baseInstance);
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
