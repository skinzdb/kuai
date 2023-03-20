#include "kpch.h"

#include "AudioManager.h"
#include "AudioClip.h"

namespace kuai {

	ALCdevice* AudioManager::device = AL_NONE;
	ALCcontext* AudioManager::context = AL_NONE;

	std::unordered_map<ALuint, ALuint*> AudioManager::sourceBufMap = std::unordered_map<ALuint, ALuint*>();
	std::unordered_map<ALuint, std::shared_ptr<AudioClip>> AudioManager::sourceClipMap = std::unordered_map<ALuint, std::shared_ptr<AudioClip>>();

	void AudioManager::init()
	{
		device = alcOpenDevice(nullptr);
		if (!device)
		{
			KU_CORE_ERROR("Failed to open playback device");
			exit(1);
		}

		KU_CORE_INFO("Started Audio Manager");
	}

	void AudioManager::cleanup()
	{
		for (auto it = sourceBufMap.begin(); it != sourceBufMap.end(); it++)
		{
			alDeleteSources(1, &it->first);
			alDeleteBuffers(BUFS_PER_SOURCE, it->second);
			delete[] it->second;
		}

		if (context)
		{
			device = alcGetContextsDevice(context);
			alcMakeContextCurrent(nullptr);
			alcDestroyContext(context);
		}

		alcCloseDevice(device);
	}

	bool AudioManager::checkAlErrors()
	{
		ALenum error = alGetError();
		if (error != AL_NO_ERROR)
		{
			switch (error)
			{
			case AL_INVALID_NAME:
				KU_CORE_ERROR("AL_INVALID_NAME: invalid ID passed to an OpenAL function");
				break;
			case AL_INVALID_ENUM:
				KU_CORE_ERROR("AL_INVALID_ENUM: invalid enum value passed to an OpenAL function");
				break;
			case AL_INVALID_VALUE:
				KU_CORE_ERROR("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
				break;
			case AL_INVALID_OPERATION:
				KU_CORE_ERROR("AL_INVALID_OPERATION: the requested operation is not valid");
				break;
			case AL_OUT_OF_MEMORY:
				KU_CORE_ERROR("AL_OUT_OF_MEMORY");
				break;
			default:
				KU_CORE_ERROR("Unknown OpenAL error occurred");
			}
			return false;
		}
		return true;
	}

	bool AudioManager::checkAlcErrors(ALCdevice* device)
	{
		ALCenum error = alcGetError(device);
		if (error != ALC_NO_ERROR)
		{
			switch (error)
			{
			case ALC_INVALID_VALUE:
				KU_CORE_ERROR("ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function");
				break;
			case ALC_INVALID_DEVICE:
				KU_CORE_ERROR("ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function");
				break;
			case ALC_INVALID_CONTEXT:
				KU_CORE_ERROR("ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
				break;
			case ALC_INVALID_ENUM:
				KU_CORE_ERROR("ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
				break;
			case ALC_OUT_OF_MEMORY:
				KU_CORE_ERROR("ALC_OUT_OF_MEMORY");
				break;
			default:
				KU_CORE_ERROR("Unknown OpenAL context error occurred");
			}
			return false;
		}
		return true;
	}

	void AudioManager::createAudioListener()
	{
		KU_CORE_ASSERT(!context, "Already created an audio listener");

		// Creating a context automatically creates a listener object
		context = alcCreateContext(device, nullptr);
		if (!alcMakeContextCurrent(context))
		{
			KU_CORE_ERROR("Failed to make audio context current");
			exit(1);
		}
	}

	void AudioManager::setListenerProperty(Property property, int val)
	{
		alListeneri(property, val);
	}

	void AudioManager::setListenerProperty(Property property, float val)
	{
		alListenerf(property, val);
	}

	void AudioManager::setListenerProperty(Property property, const std::vector<float>& vals)
	{
		alListenerfv(property, vals.data());
	}

	void AudioManager::setListenerProperty(Property property, const glm::vec3& val)
	{
		alListener3f(property, val.x, val.y, val.z);
	}

	uint32_t AudioManager::createAudioSource()
	{
		ALuint source;
		alGenSources(1, &source);

		ALuint* bufs = new ALuint[BUFS_PER_SOURCE];	// Have X buffers per audio source for streaming
		alGenBuffers(BUFS_PER_SOURCE, bufs);

		sourceBufMap.insert(std::pair<ALuint, ALuint*>(source, bufs));

		return source;
	}

	void AudioManager::playAudioSource(ALuint sourceId)
	{
		alSourcePlay(sourceId);
	}

	void AudioManager::stopAudioSource(ALuint sourceId)
	{
		alSourceStop(sourceId);
	}

	void AudioManager::setSourceProperty(ALuint sourceId, Property property, int val)
	{
		alSourcei(sourceId, property, val);
	}

	void AudioManager::setSourceProperty(ALuint sourceId, Property property, float val)
	{
		alSourcef(sourceId, property, val);
	}

	void AudioManager::setSourceProperty(ALuint sourceId, Property property, const glm::vec3& val)
	{
		alSource3f(sourceId, property, val.x, val.y, val.z);
	}

	void AudioManager::setSourceAudioClip(ALuint sourceId, std::shared_ptr<AudioClip> clip)
	{
		sourceClipMap[sourceId] = clip;

		// *******************************************************
		// TODO: streaming audio !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// *******************************************************

		//if (BUFS_PER_SOURCE * BUF_SIZE > clip->data.size())
		//{
		alBufferData(sourceBufMap[sourceId][0], clip->format, &clip->data[0], clip->data.size() * 2, clip->samplerate);
		alSourcei(sourceId, AL_BUFFER, sourceBufMap[sourceId][0]);
		//	return;
		//}

		//for (size_t i = 0; i < BUFS_PER_SOURCE; i++) 
		//{
		//	alBufferData(sourceBufMap[sourceId][i], clip->format, &clip->data[i * BUF_SIZE], BUF_SIZE * 2, clip->samplerate); // BUF_SIZE * 2 because OpenAL expects bytes and we are using shorts
		//}

		//clip->cursor = BUFS_PER_SOURCE * BUF_SIZE;
		//alSourceQueueBuffers(sourceId, BUFS_PER_SOURCE, &sourceBufMap[sourceId][0]);
	}

	void AudioManager::updateStream(ALuint sourceId)
	{
		//ALint buffersProcessed = 0;
		//alGetSourcei(sourceId, AL_BUFFERS_PROCESSED, &buffersProcessed);

		//if (buffersProcessed <= 0)
		//	return;

		//auto clip = sourceClipMap[sourceId];

		//while (buffersProcessed--)
		//{
		//	ALuint bufferId;
		//	alSourceUnqueueBuffers(sourceId, 1, &bufferId); // Pop buffer

		//	short* data = new short[BUF_SIZE];

		//	size_t copySize = BUF_SIZE;
		//	if (clip->cursor + BUF_SIZE > clip->data.size()) // Do not copy past the end of the sound data
		//		copySize = clip->data.size() - clip->cursor;

		//	std::memcpy(&data[0], &clip->data[clip->cursor], copySize * 2); // Copy sound data into data buffer (copySize * 2 because function uses bytes not shorts)

		//	clip->cursor += copySize;

		//	if (copySize < BUF_SIZE) // If data buffer not full, copy the remaining amount from the start of sound data
		//	{
		//		clip->cursor = 0;
		//		std::memcpy(&data[copySize], &clip->data[clip->cursor], std::min(BUF_SIZE - copySize, clip->data.size()) * 2);
		//		clip->cursor = (BUF_SIZE - copySize) % clip->data.size();
		//	}

		//	alBufferData(bufferId, clip->format, data, BUF_SIZE * 2, clip->samplerate);
		//	alSourceQueueBuffers(sourceId, 1, &bufferId);

		//	delete[] data;
		//}
	}
}