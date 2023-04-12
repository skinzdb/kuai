#pragma once

// Forward declaration
class SndfileHandle;

namespace kuai {
	class AudioClip
	{
	public:
		AudioClip(const std::string& filename);
		// TODO: AudioClip(const std::string& filename, bool stream = false); // Move stream option to audioclip
		~AudioClip();

		int getSamplerate();
		int getChannels();
		int getFrames();

	private:
		uint64_t loadData(short* dest, uint64_t amount);
		std::vector<short> loadAllData();

		int getFormat();

		SndfileHandle* handle = nullptr;

		friend class AudioSource;
		friend class MusicSource;
	};
}

