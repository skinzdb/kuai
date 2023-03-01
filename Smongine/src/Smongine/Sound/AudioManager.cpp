#include "smpch.h"
#include "AudioManager.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace Smong {
	AudioManager* AudioManager::instance = nullptr;

	bool AudioManager::CheckAlErrors()
	{
		ALenum error = alGetError();
		if (error != AL_NO_ERROR)
		{
			switch (error)
			{
			case AL_INVALID_NAME:
				SM_CORE_ERROR("AL_INVALID_NAME: invalid ID passed to an OpenAL function");
				break;
			case AL_INVALID_ENUM:
				SM_CORE_ERROR("AL_INVALID_ENUM: invalid enum value passed to an OpenAL function");
				break;
			case AL_INVALID_VALUE:
				SM_CORE_ERROR("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
				break;
			case AL_INVALID_OPERATION:
				SM_CORE_ERROR("AL_INVALID_OPERATION: the requested operation is not valid");
				break;
			case AL_OUT_OF_MEMORY:
				SM_CORE_ERROR("AL_OUT_OF_MEMORY");
				break;
			default:
				SM_CORE_ERROR("Unknown OpenAL error occurred");
			}
			return false;
		}
		return true;
	}

	bool AudioManager::CheckAlcErrors(ALCdevice* device)
	{
		ALCenum error = alcGetError(device);
		if (error != ALC_NO_ERROR)
		{
			switch (error)
			{
			case ALC_INVALID_VALUE:
				SM_CORE_ERROR("ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function");
				break;
			case ALC_INVALID_DEVICE:
				SM_CORE_ERROR("ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function");
				break;
			case ALC_INVALID_CONTEXT:
				SM_CORE_ERROR("ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
				break;
			case ALC_INVALID_ENUM:
				SM_CORE_ERROR("ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
				break;
			case ALC_OUT_OF_MEMORY:
				SM_CORE_ERROR("ALC_OUT_OF_MEMORY");
				break;
			default:
				SM_CORE_ERROR("Unknown OpenAL context error occurred");
			}
			return false;
		}
		return true;
	}

	AudioManager::AudioManager()
	{
		SM_CORE_ASSERT(!instance, "Audio manager already exists");
		instance = this;

		device = alcOpenDevice(nullptr);
		if (!device)
		{
			SM_CORE_ERROR("Failed to open playback device");
			exit(1);
		}

		context = alcCreateContext(device, nullptr);
		if (!alcMakeContextCurrent(context))
		{
			SM_CORE_ERROR("Failed to make audio context current");
			exit(1);
		}

		SM_CORE_TRACE("Started Audio Manager");
	}

	AudioManager::~AudioManager()
	{
		alDeleteSources(sourceIds.size(), sourceIds.data());
		
		device = alcGetContextsDevice(context);
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	uint32_t AudioManager::CreateAudioSource()
	{
		ALuint source;
		alGenSources(1, &source);
		alSourcef(source, AL_PITCH, 1.0f);
		alSourcef(source, AL_GAIN, 1.0f);

		sourceIds.push_back(source);

		return source;
	}

	void AudioManager::PlayAudioSource(uint32_t sourceId)
	{
		alSourcePlay(sourceId);
	}

	void AudioManager::Update()
	{

	}
}