#include "smpch.h"
#include "AudioClip.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <sndfile.hh>


namespace Smong {
	AudioClip::AudioClip(std::string& filename)
	{
		alGenBuffers(1, &bufferId);

		int size, freq;
		ALenum format;
		ALvoid* data;
		ALboolean loop = AL_FALSE;

		SndfileHandle file;

		short buffer[1024];

		file = SndfileHandle(filename);

		file.read(buffer, 1024);
		
		//alBufferData(buffer, file.format, data, 0, file.samplerate);

	}
	AudioClip::~AudioClip()
	{

	}
}