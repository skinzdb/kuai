#include "kpch.h"
#include "AudioClip.h"

#include "AudioManager.h"

#include <sndfile.hh>

namespace kuai {

	AudioClip::AudioClip(const std::string& filename)
	{
		KU_PROFILE_FUNCTION();

		SndfileHandle file(filename);

		if (file.channels() == 1 && file.format() & SF_FORMAT_PCM_S8)
			format = AL_FORMAT_MONO8;
		else if (file.channels() == 1 && file.format() & SF_FORMAT_PCM_16)
			format = AL_FORMAT_MONO16;
		else if (file.channels() == 2 && file.format() & SF_FORMAT_PCM_S8)
		{
			format = AL_FORMAT_STEREO8;
			KU_CORE_WARN("Sound file {0} is stereo and will not have 3D positional sound", filename);
		}
		else if (file.channels() == 2 && file.format() & SF_FORMAT_PCM_16)
		{
			format = AL_FORMAT_STEREO16;
			KU_CORE_WARN("Sound file {0} is stereo and will not have 3D positional sound", filename);
		}

		samplerate = file.samplerate();

		data.resize(file.frames() * file.channels());
		file.read(data.data(), file.frames() * file.channels());
	}

	AudioClip::~AudioClip()
	{
	
	}
}