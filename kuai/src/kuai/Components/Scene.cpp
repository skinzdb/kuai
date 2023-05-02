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
		mainLight->getComponent<Light>().setShadows(true);
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

		Rc<Model> model = scene->getEntityById(entity)->getComponent<MeshRenderer>().getModel();

		// For every mesh in the model
		for (size_t i = 0; i < model->getMeshes().size(); i++)
		{
			Rc<Mesh> mesh = model->getMeshes()[i];
			u32 id = mesh->getId();
			Shader* shader = model->getMaterials()[i]->getShader();

			IndirectCommand& cmd = shaderToMeshCommand[shader][id];
			cmd.instanceCount = ++shaderMeshToNumInstances[shader][id]; // Increment instance count by one

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

			shaderToEntities[shader][entity] = 1;

			shaderToMeshCommand[shader][id] = cmd;

			shaderToInstances[shader]++;
		}

		// Resize model matrices and set commands
		for (auto& pair : shaderToInstances)
			pair.first->getVertexArray()->getVertexBuffers()[1]->reset(nullptr, pair.second * sizeof(glm::mat4), DrawHint::DYNAMIC);
		setCommands();
	}

	void RenderSystem::removeEntity(EntityID entity)
	{
		Rc<Model> model = scene->getEntityById(entity)->getComponent<MeshRenderer>().getModel();

		for (size_t i = 0; i < model->getMeshes().size(); i++)
		{
			Rc<Mesh> mesh = model->getMeshes()[i];
			u32 id = mesh->getId();
			Shader* shader = model->getMaterials()[i]->getShader();

			IndirectCommand& cmd = shaderToMeshCommand[shader][id];
			cmd.instanceCount = --shaderMeshToNumInstances[shader][id]; // Decrement instance count by one

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
				shaderMeshToNumInstances[shader].erase(id);

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

			shaderToEntities[shader].erase(entity);

			shaderToInstances[shader]--;
		}

		// Resize model matrices and set commands
		for (auto& pair : shaderToInstances)
			pair.first->getVertexArray()->getVertexBuffers()[1]->reset(nullptr, pair.second * sizeof(glm::mat4), DrawHint::DYNAMIC);
		setCommands();

		System::removeEntity(entity);
	}

	void RenderSystem::update(float dt)
	{
		// Update model matrices every frame
		// TODO: inefficient, only update when transform moves

		for (auto& pair : shaderToEntities)
		{
			std::vector<glm::mat4> modelMatrices;
			for (auto& innerPair : pair.second)
				modelMatrices.push_back(scene->getEntityById(innerPair.first)->getTransform().getModelMatrix());
			shaderToModelMatrices[pair.first] = modelMatrices;
		}
	}

	void RenderSystem::render()
	{
		//KU_PROFILE_FUNCTION();

		// ---- SHADOWS ----

		if (scene->getMainLight().castsShadows())
		{
			auto& basicShaderVertexData = shaderToVertexData[StaticShader::basic];
			auto& basicShaderIndices = shaderToIndices[StaticShader::basic];
			auto& basicShaderModelMatrices = shaderToModelMatrices[StaticShader::basic];

			StaticShader::depth->bind();

			StaticShader::depth->getVertexArray()->getVertexBuffers()[0]->reset(basicShaderVertexData.data(), basicShaderVertexData.size() * sizeof(Vertex));
			StaticShader::depth->getVertexArray()->setIndexBuffer(MakeRc<IndexBuffer>(basicShaderIndices.data(), basicShaderIndices.size()));
			StaticShader::depth->getVertexArray()->getVertexBuffers()[1]->reset(basicShaderModelMatrices.data(), basicShaderModelMatrices.size() * sizeof(glm::mat4));

			std::vector<IndirectCommand> commands;
			for (auto& pair : shaderToMeshCommand[StaticShader::basic])
				commands.push_back(pair.second);
			StaticShader::depth->setIndirectBufData(commands);

			Renderer::updateShadowMap(scene->getMainLight());
		}

		// -----------------

		Renderer::setViewport(0, 0, App::get().getWindow().getWidth(), App::get().getWindow().getHeight());
		Renderer::clear();

		for (auto& pair : shaderToInstances)
		{
			Shader* shader = pair.first;
			shader->bind();

			u32 offset = 0;
			for (auto& pair : shaderToEntities[shader])
			{
				Rc<Entity> entity = scene->getEntityById(pair.first);
				Rc<Model> model = entity->getComponent<MeshRenderer>().getModel();

				for (int i = 0; i < model->getMeshes().size(); i++)
				{
					// TODO: change this to a big texture array or something that doesn't send possibly duplicate materials
					Rc<Mesh> mesh = model->getMeshes()[i];
					Rc<Material> material = model->getMaterials()[i];

					if (shader == StaticShader::skybox)
					{
						material->bind(0);
						goto RENDER_LABEL;
					}

					if (offset >= 10)
						goto RENDER_LABEL;

					material->bind(offset * 3);

					offset++;
				}
			}

		RENDER_LABEL:

			if (dataChanged)
			{
				auto& vertexData = shaderToVertexData[shader];
				shader->getVertexArray()->getVertexBuffers()[0]->reset(vertexData.data(), vertexData.size() * sizeof(Vertex), DrawHint::DYNAMIC);

				auto& indices = shaderToIndices[shader];

				shader->getVertexArray()->setIndexBuffer(MakeRc<IndexBuffer>(indices.data(), indices.size()));
			}
			else
			{
				auto& modelMatrices = shaderToModelMatrices[shader];
				shader->getVertexArray()->getVertexBuffers()[1]->setData(modelMatrices.data(), modelMatrices.size() * sizeof(glm::mat4));
			}

			Renderer::render(shader);
		}
		dataChanged = false;
	}
	
	void RenderSystem::setCommands()
	{
		int i = 0;
		for (auto& pair : shaderToMeshCommand)
		{
			//KU_CORE_ERROR("Shader {0}:", i);
			pair.first->bind();
			std::vector<IndirectCommand> commands;
			for (auto& innerPair : pair.second)
			{
				commands.push_back(innerPair.second);
				//KU_CORE_ERROR("*** count:{0}, no. instances:{1}, vert offset:{2}, index offset:{3}, base instance:{4} ***", innerPair.second.count, innerPair.second.instanceCount, innerPair.second.firstIndex, innerPair.second.baseVertex, innerPair.second.baseInstance);
			}
			
			pair.first->setIndirectBufData(commands);
			i++;
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

				StaticShader::basic->setUniform("lightSpaceMatrix", scene->getMainLight().getLightSpaceMatrix());

				l.changed = false;
			}
		}

		int numLights = entities.size();
		StaticShader::basic->setUniform("Lights", "numLights", &numLights, sizeof(int));
	}
}
