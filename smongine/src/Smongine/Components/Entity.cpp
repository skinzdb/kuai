#include "smpch.h"
#include "Entity.h"

namespace Smong {

	Entity::Entity(std::shared_ptr<EntityComponentSystem> ECS)
	{
		this->ECS = ECS;
		id = ECS->CreateEntity();
		transform = AddComponent<Transform>(); // Every base object will have a transform component
	}

	Entity::Entity(std::shared_ptr<EntityComponentSystem> ECS, EntityID id)
	{
		this->ECS = ECS;
		this->id = id;
		transform = GetComponent<Transform>();
	}

	Transform& Entity::GetTransform()
	{
		return transform;
	}

	EntityID Entity::GetId() const
	{
		return id;
	}

	void Entity::Destroy()
	{
		ECS->DestroyEntity(id);
	}

}