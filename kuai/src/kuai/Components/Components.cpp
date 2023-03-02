#include "kpch.h"
#include "Components.h"

#include "Entity.h"
#include "kuai/Renderer/Renderer.h"
//#include "kuai/Sound/AudioManager.h"

namespace kuai {
	Transform& Component::getTransform()
	{
		return entity->getTransform();
	}

	void Transform::updateComponents()
	{	
		if (hasComponent<Camera>())
			getComponent<Camera>().updateViewMatrix(pos, rot);
	}

	void Camera::updateViewMatrix(glm::vec3 pos, glm::vec3 rot)
	{
		viewMatrix = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(glm::quat(rot)); // Rotate then translate, aka TR
		viewMatrix = glm::inverse(viewMatrix); // Calculate inverse to get correct operation, aka (TR)^-1 = R^-1T^-1
		Renderer::setCamera(*this, getTransform().getPos());
	}

	void Camera::updateProjectionMatrix()
	{
		if (projectionType == ProjectionType::Perspective)
		{
			projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
		}
		else
		{
			projectionMatrix = glm::ortho(-orthoSize * aspect * 0.5f, orthoSize * aspect * 0.5f, orthoNear, orthoFar);
		}
		Renderer::setCamera(*this, getTransform().getPos());
	}

	AudioSource::AudioSource(Entity* entity) : Component(entity)
	{
		//sourceId = AudioManager::get().createAudioSource();
	}

	void AudioSource::play()
	{
		//AudioManager::get().PlayAudioSource(sourceId);
	}

	void AudioSource::stop()
	{
	}
}