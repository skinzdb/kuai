#include "kpch.h"
#include "Components.h"

#include "Entity.h"

#include "kuai/Renderer/Renderer.h"

#include "kuai/Sound/AudioManager.h"
#include "kuai/Sound/AudioSource.h"
#include "kuai/Sound/MusicSource.h"

namespace kuai {
	uint32_t LightCounter::lightCount = 0;

	Transform& Component::getTransform()
	{
		return entity->getTransform();
	}

	void Transform::updateComponents()
	{	
		if (hasComponent<Camera>())
		{
			getComponent<Camera>().updateViewMatrix(pos, rot);
		}

		if (hasComponent<AudioListener>())
		{
			getComponent<AudioListener>().update();
		}

		if (hasComponent<AudioSourceComponent>())
		{
			getComponent<AudioSourceComponent>().update();
		}
	}

	void Camera::updateViewMatrix(glm::vec3 pos, glm::vec3 rot)
	{
		viewMatrix = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(glm::quat(rot)); // Rotate then translate, aka TR
		viewMatrix = glm::inverse(viewMatrix); // Calculate inverse to get correct operation, aka (TR)^-1 = R^-1T^-1

		changed = true;
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
		changed = true;
	}

	AudioListener::AudioListener(Entity* entity) : Component(entity)
	{
	}

	void AudioListener::update()
	{
		AudioManager::setPos(getTransform().getPos());
		AudioManager::setOrientation(getTransform().getForward(), getTransform().getUp());
	}

	AudioSourceComponent::AudioSourceComponent(Entity* entity, bool stream) : Component(entity)
	{
		source = AudioManager::createAudioSource(stream);
	}

	void AudioSourceComponent::update()
	{
		source->setPos(getTransform().getPos());
		source->setDir(getTransform().getForward());
	}
}