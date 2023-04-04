#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <glm/glm.hpp>

namespace kuai {
	const uint32_t BUFS_PER_SOURCE = 4;
	const uint32_t BUF_SIZE = 32768;

	// Forward Declaration
	class AudioClip;

	class AudioManager
	{
	public:
		enum Property
		{
			Pitch		= AL_PITCH,
			Gain		= AL_GAIN,
			MaxDist		= AL_MAX_DISTANCE,
			Rolloff		= AL_ROLLOFF_FACTOR,
			RefDist     = AL_REFERENCE_DISTANCE,
			Position	= AL_POSITION,
			Direction	= AL_DIRECTION,
			Orientation = AL_ORIENTATION,
			Velocity	= AL_VELOCITY,
			Loop		= AL_LOOPING,
			Buffer		= AL_BUFFER,
			State		= AL_SOURCE_STATE
		};

		static void init();
		static void cleanup();

		static void createAudioListener();

		static void setListenerProperty(Property property, int val);
		static void setListenerProperty(Property property, float val);
		static void setListenerProperty(Property property, const glm::vec3& val);
		static void setListenerProperty(Property property, const std::vector<float>& vals);

		static ALuint createAudioSource();
		static void playAudioSource(ALuint sourceId);
		static void stopAudioSource(ALuint sourceId);

		static void setSourceProperty(ALuint sourceId, Property property, int val);
		static void setSourceProperty(ALuint sourceId, Property property, float val);
		static void setSourceProperty(ALuint sourceId, Property property, const glm::vec3& val);

		static void setSourceAudioClip(ALuint sourceId, std::shared_ptr<AudioClip> audioClip);

		static void updateStream(ALuint sourceId);

	private:
		static ALCdevice* device;
		static ALCcontext* context;

		static std::unordered_map<ALuint, ALuint*> sourceBufMap;
		static std::unordered_map<ALuint, std::shared_ptr<AudioClip>> sourceClipMap;
	};
	
	// SFML - Simple and Fast Multimedia Library
	// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
	// The do-while loop is needed so that alCheck can be used as a single statement in if/else branches
	//#ifdef KU_DEBUG
		#define alCheck(expr)                                      \
			do                                                     \
			{                                                      \
				expr;                                              \
				kuai::checkAlErrors(__FILE__, __LINE__, #expr);	   \
			} while (false)
	//#else
	//	#define alCheck(expr) expr
	//#endif

	void checkAlErrors(const std::filesystem::path& file, unsigned int line, std::string_view expression);
}


