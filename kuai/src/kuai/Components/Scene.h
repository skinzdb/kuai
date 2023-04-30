#pragma once

#include "EntityComponentSystem.h"
#include "Entity.h"
#include "Components.h"

namespace kuai {
	// Forward declarations
	class RenderSystem;
	class CameraSystem;
	class LightSystem;
	
	/** \class Scene
	*	\brief Represents a game scene by holding a list of entity objects. The scene owns a main camera; the view of this camera gets rendered to the viewport.
	*	The scene owns a collection of systems that update game logic.
		@see Entity  
	*/
	class Scene
	{
	public:
		Scene();
		~Scene();

		/** 
		* Adds new entity to scene.
		*/
		Rc<Entity> createEntity();

		/**
		* Finds an entity by its ID and returns a reference if it exists.
		*/
		Rc<Entity> getEntityById(EntityID entity);
		Rc<Entity> getEntityByName(const std::string& name) { KU_CORE_ASSERT(0, "Not yet implemented"); } // TODO: implement
		/**
		* Removes entity from the scene.
		*/
		void destroyEntity(EntityID entity);

		/**
		* Notify all systems of an event.
		* @param event Event that gets sent to subscribing systems.
		*/
		template<typename EventType>
		void notifySystems(EventType* event)
		{
			ECS->notifySystems(event);
		}

		/**
		* Subscribe to a callback function that gets activated whenever its event happens.
		* @param instance The instance that owns the function.
		* @param memberFn The callback function.
		*/
		template<typename T, typename EventType>
		void subscribeSystem(T* instance, void (T::* memberFn)(EventType*))
		{
			ECS->subscribeSystem(instance, memberFn);
		}

		CameraComponent& getMainCam();
		void setMainCam(CameraComponent& cam);

		Light& getMainLight();
		void setMainLight(Light& light);

		std::unordered_map<u32, Rc<Entity>>::iterator begin() { return entityMap.begin(); }
		std::unordered_map<u32, Rc<Entity>>::iterator end() { return entityMap.end(); }

		void update(float dt);
	private:
		Rc<Entity> mainCam;
		Rc<Entity> mainLight;

		EntityComponentSystem* ECS;
		std::unordered_map<u32, Rc<Entity>> entityMap;

		Rc<RenderSystem> renderSys;
		Rc<CameraSystem> cameraSys;
		Rc<LightSystem> lightSys;
	};

	class RenderSystem : public System
	{
	public:
		RenderSystem(Scene* scene) : System(scene) { acceptsSubset = true; }

		virtual void insertEntity(EntityID entity) override;
		virtual void removeEntity(EntityID entity) override;
		virtual void update(float dt) override;

		void render();

	private:
		void setCommands();

	private:
		// Total number of instances shader owns
		std::unordered_map<Shader*, size_t> shaderToInstances;

		// Maps meshes to the number of instances they have.
		std::unordered_map<u32, size_t> meshToNumInstances;

		// Map each shader to a list of vertices, indices and model matrices.
		std::unordered_map<Shader*, std::vector<Vertex>> shaderToVertexData;
		std::unordered_map<Shader*, std::vector<u32>> shaderToIndices;
		std::unordered_map<Shader*, std::vector<glm::mat4>> shaderToModelMatrices;

		// For each shader and for each mesh, store the sizes of their vertex and index lists.
		std::unordered_map<Shader*, std::unordered_map<u32, size_t>> shaderToVertexDataSizes;
		std::unordered_map<Shader*, std::unordered_map<u32, size_t>> shaderToIndicesSizes;
	
		// For each shader and for each mesh, store its corresponding render command.
		std::unordered_map<Shader*, std::unordered_map<u32, IndirectCommand>> shaderToMeshCommand;

		bool dataChanged = false;
	};

	class CameraSystem : public System
	{
	public:
		CameraSystem(Scene* scene) : System(scene) { acceptsSubset = true; }

		void setRenderSystem(Rc<RenderSystem> renderSys) { this->renderSys = renderSys; }
		void setMainCam(CameraComponent& cam);

		virtual void update(float dt) override;

	private:
		Rc<RenderSystem> renderSys;
	};

	class LightSystem : public System
	{
	public:
		LightSystem(Scene* scene) : System(scene) { acceptsSubset = true; }
		
		void setRenderSystem(Rc<RenderSystem> renderSys) { this->renderSys = renderSys; }

		virtual void update(float dt) override;

	private:
		Rc<RenderSystem> renderSys;
	};

	//class PhysicsSystem : public System
	//{
	//	virtual void update(float dt) override
	//	{
	//		for (auto& entity : entities)
	//		{
	//			Rigidbody rigidbody = entity.getComponent<Rigidbody>();

	//			entity.getTransform().pos += rigidbody.velocity * dt;
	//			
	//			if (rigidbody.useGravity)
	//			{
	//				rigidbody.velocity -= entity.getTransform().getUp() * dt; // Add gravitational force var
	//			}
	//		}
	//	}
	//};
}
