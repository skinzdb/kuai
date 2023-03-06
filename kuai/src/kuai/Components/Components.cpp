#include "kpch.h"
#include "Components.h"

#include "Entity.h"
#include "kuai/Renderer/Renderer.h"
#include "kuai/Sound/AudioManager.h"

namespace kuai {
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
			getComponent<AudioListener>().setPos(pos);
			getComponent<AudioListener>().setOrientation(getForward(), getUp());
		}

		if (hasComponent<AudioSource>())
		{
			getComponent<AudioSource>().setPos(pos);
			getComponent<AudioSource>().setDir(getForward());
		}
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
		sourceId = AudioManager::createAudioSource();
		setPitch(pitch);
		setGain(gain);
		setPos(getTransform().getPos());
		setDir(getTransform().getForward());
	}

	void AudioSource::play()
	{
		AudioManager::playAudioSource(sourceId);
	}

	void AudioSource::stop()
	{
		AudioManager::stopAudioSource(sourceId);
	}

	void AudioSource::setAudioClip(std::shared_ptr<AudioClip> audioClip)
	{
		this->audioClip = audioClip;
		AudioManager::setSourceAudioClip(sourceId, audioClip);
	}

	void AudioSource::setGain(float gain)
	{
		this->gain = gain;
		AudioManager::setSourceProperty(sourceId, AudioManager::Gain, gain);
	}

	void AudioSource::setPitch(float pitch)
	{
		this->pitch = pitch;
		AudioManager::setSourceProperty(sourceId, AudioManager::Pitch, pitch);
	}

	void AudioSource::setRolloff(float rolloff)
	{
		this->rolloff = rolloff;
		AudioManager::setSourceProperty(sourceId, AudioManager::Rolloff, rolloff);
	}

	void AudioSource::setRefDist(float refDist)
	{
		this->refDist = refDist;
		AudioManager::setSourceProperty(sourceId, AudioManager::RefDist, refDist);
	}

	void AudioSource::setLoop(bool loop)
	{
		this->loop = loop;
		AudioManager::setSourceProperty(sourceId, AudioManager::Loop, loop);
	}

	void AudioSource::setPos(glm::vec3& pos)
	{
		AudioManager::setSourceProperty(sourceId, AudioManager::Position, pos);
	}

	void AudioSource::setDir(glm::vec3& dir)
	{
		AudioManager::setSourceProperty(sourceId, AudioManager::Direction, dir);
	}

	void AudioSource::setVel(glm::vec3& vel)
	{
		AudioManager::setSourceProperty(sourceId, AudioManager::Velocity, vel);
	}

	AudioListener::AudioListener(Entity* entity) : Component(entity)
	{
		AudioManager::createAudioListener();
		setGain(gain);
		setPos(getTransform().getPos());
		setOrientation(getTransform().getForward(), getTransform().getUp());
	}

	void AudioListener::setGain(float gain)
	{
		this->gain = gain;
		AudioManager::setListenerProperty(AudioManager::Gain, gain);
	}

	void AudioListener::setPos(glm::vec3& pos)
	{
		AudioManager::setListenerProperty(AudioManager::Position, pos);
	}

	void AudioListener::setOrientation(glm::vec3& at, glm::vec3& up)
	{
		std::vector<float> data = { at.x, at.y, at.z, up.x, up.y, up.z };
		AudioManager::setListenerProperty(AudioManager::Orientation, data);
	}

	void AudioListener::setVel(glm::vec3& vel)
	{
		AudioManager::setListenerProperty(AudioManager::Velocity, vel);
	}

}