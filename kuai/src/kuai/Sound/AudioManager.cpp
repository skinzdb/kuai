#include "kpch.h"

#include "AudioManager.h"
#include "AudioClip.h"

#include <filesystem>

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
			alCheck(alDeleteSources(1, &it->first));
			alCheck(alDeleteBuffers(BUFS_PER_SOURCE, it->second));
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
		alCheck(alListeneri(property, val));
	}

	void AudioManager::setListenerProperty(Property property, float val)
	{
		alCheck(alListenerf(property, val));
	}

	void AudioManager::setListenerProperty(Property property, const std::vector<float>& vals)
	{
		alCheck(alListenerfv(property, vals.data()));
	}

	void AudioManager::setListenerProperty(Property property, const glm::vec3& val)
	{
		alCheck(alListener3f(property, val.x, val.y, val.z));
	}

	uint32_t AudioManager::createAudioSource()
	{
		ALuint source;
		alCheck(alGenSources(1, &source));

		ALuint* bufs = new ALuint[BUFS_PER_SOURCE];	// Have X buffers per audio source for streaming
		alCheck(alGenBuffers(BUFS_PER_SOURCE, bufs));

		sourceBufMap.insert(std::pair<ALuint, ALuint*>(source, bufs));

		return source;
	}

	void AudioManager::playAudioSource(ALuint sourceId)
	{
		alCheck(alSourcePlay(12));
	}

	void AudioManager::stopAudioSource(ALuint sourceId)
	{
		alCheck(alSourceStop(sourceId));
	}

	void AudioManager::setSourceProperty(ALuint sourceId, Property property, int val)
	{
		alCheck(alSourcei(sourceId, property, val));
	}

	void AudioManager::setSourceProperty(ALuint sourceId, Property property, float val)
	{
		alCheck(alSourcef(sourceId, property, val));
	}

	void AudioManager::setSourceProperty(ALuint sourceId, Property property, const glm::vec3& val)
	{
		alCheck(alSource3f(sourceId, property, val.x, val.y, val.z));
	}

	void AudioManager::setSourceAudioClip(ALuint sourceId, std::shared_ptr<AudioClip> clip)
	{
		sourceClipMap[sourceId] = clip;

		// *******************************************************
		// TODO: streaming audio !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// *******************************************************

		//if (BUFS_PER_SOURCE * BUF_SIZE > clip->data.size())
		//{
		alCheck(alBufferData(sourceBufMap[sourceId][0], clip->format, &clip->data[0], clip->data.size() * 2, clip->samplerate));
		alCheck(alSourcei(sourceId, AL_BUFFER, sourceBufMap[sourceId][0]));
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

	/*bool AudioManager::checkAlcErrors(ALCdevice* device)
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
	}*/

	void checkAlErrors(const std::filesystem::path& file, unsigned int line, std::string_view expression)
	{
		// Get the last error
		ALenum errorCode = alGetError();

		if (errorCode != AL_NO_ERROR)
		{
			std::string error = "Unknown error";
			std::string description = "No description";

			// Decode the error code
			switch (errorCode)
			{
			case AL_INVALID_NAME:
			{
				error = "AL_INVALID_NAME";
				description = "A bad name (ID) has been specified.";
				break;
			}

			case AL_INVALID_ENUM:
			{
				error = "AL_INVALID_ENUM";
				description = "An unacceptable value has been specified for an enumerated argument.";
				break;
			}

			case AL_INVALID_VALUE:
			{
				error = "AL_INVALID_VALUE";
				description = "A numeric argument is out of range.";
				break;
			}

			case AL_INVALID_OPERATION:
			{
				error = "AL_INVALID_OPERATION";
				description = "The specified operation is not allowed in the current state.";
				break;
			}

			case AL_OUT_OF_MEMORY:
			{
				error = "AL_OUT_OF_MEMORY";
				description = "There is not enough memory left to execute the command.";
				break;
			}
			}

			// Log the error
			KU_CORE_ERROR("An internal OpenAL call failed in {0}({1}).\nExpression:\n  {2}\nError Description:\n  {3} - {4}",
				file.filename(), line, expression, error, description);
		}
	}
}