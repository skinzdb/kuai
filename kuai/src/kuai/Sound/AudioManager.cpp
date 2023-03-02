#include "kpch.h"
//#include "AudioManager.h"
//
//#include <AL/al.h>
//#include <AL/alc.h>
//
//namespace kuai {
//
//	ALCdevice* AudioManager::device = 0;
//	ALCcontext* AudioManager::context = 0;
//
//	std::vector<uint32_t>  AudioManager::sourceIds = std::vector<uint32_t>();
//
//	void AudioManager::init()
//	{
//		device = alcOpenDevice(nullptr);
//		if (!device)
//		{
//			KU_CORE_ERROR("Failed to open playback device");
//			exit(1);
//		}
//
//		context = alcCreateContext(device, nullptr);
//		if (!alcMakeContextCurrent(context))
//		{
//			KU_CORE_ERROR("Failed to make audio context current");
//			exit(1);
//		}
//
//		KU_CORE_TRACE("Started Audio Manager");
//	}
//
//	void AudioManager::cleanup()
//	{
//		alDeleteSources(sourceIds.size(), sourceIds.data());
//
//		device = alcGetContextsDevice(context);
//		alcMakeContextCurrent(nullptr);
//		alcDestroyContext(context);
//		alcCloseDevice(device);
//	}
//
//	bool AudioManager::checkAlErrors()
//	{
//		ALenum error = alGetError();
//		if (error != AL_NO_ERROR)
//		{
//			switch (error)
//			{
//			case AL_INVALID_NAME:
//				KU_CORE_ERROR("AL_INVALID_NAME: invalid ID passed to an OpenAL function");
//				break;
//			case AL_INVALID_ENUM:
//				KU_CORE_ERROR("AL_INVALID_ENUM: invalid enum value passed to an OpenAL function");
//				break;
//			case AL_INVALID_VALUE:
//				KU_CORE_ERROR("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
//				break;
//			case AL_INVALID_OPERATION:
//				KU_CORE_ERROR("AL_INVALID_OPERATION: the requested operation is not valid");
//				break;
//			case AL_OUT_OF_MEMORY:
//				KU_CORE_ERROR("AL_OUT_OF_MEMORY");
//				break;
//			default:
//				KU_CORE_ERROR("Unknown OpenAL error occurred");
//			}
//			return false;
//		}
//		return true;
//	}
//
//	bool AudioManager::checkAlcErrors(ALCdevice* device)
//	{
//		ALCenum error = alcGetError(device);
//		if (error != ALC_NO_ERROR)
//		{
//			switch (error)
//			{
//			case ALC_INVALID_VALUE:
//				KU_CORE_ERROR("ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function");
//				break;
//			case ALC_INVALID_DEVICE:
//				KU_CORE_ERROR("ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function");
//				break;
//			case ALC_INVALID_CONTEXT:
//				KU_CORE_ERROR("ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
//				break;
//			case ALC_INVALID_ENUM:
//				KU_CORE_ERROR("ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
//				break;
//			case ALC_OUT_OF_MEMORY:
//				KU_CORE_ERROR("ALC_OUT_OF_MEMORY");
//				break;
//			default:
//				KU_CORE_ERROR("Unknown OpenAL context error occurred");
//			}
//			return false;
//		}
//		return true;
//	}
//
//	uint32_t AudioManager::createAudioSource()
//	{
//		ALuint source;
//		alGenSources(1, &source);
//		alSourcef(source, AL_PITCH, 1.0f);
//		alSourcef(source, AL_GAIN, 1.0f);
//
//		sourceIds.push_back(source);
//
//		return source;
//	}
//
//	void AudioManager::playAudioSource(uint32_t sourceId)
//	{
//		alSourcePlay(sourceId);
//	}
//
//	void AudioManager::update()
//	{
//
//	}
//}