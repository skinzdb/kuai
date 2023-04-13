#pragma once

// Forward declaration
class SndfileHandle;

namespace kuai {
	/** \class AudioClip
	*	\brief Stores an audio file from a given location.
	*/
	class AudioClip
	{
	public:
		/**
		* Creates audio clip from a sound file.
		*/
		AudioClip(const std::string& filename);
		// TODO: AudioClip(const std::string& filename, bool stream = false); // Move stream option to audioclip
		~AudioClip();

		int getSamplerate();
		int getChannels();
		int getFrames();

	private:
		/**
		* Load (amount * sizeof(short)) bytes of data from file into dest.
		* @return The number of shorts read from the file.
		*/
		uint64_t loadData(short* dest, uint64_t amount);
		/**
		* Load the whole file at once and return it as a vector of shorts.
		*/
		std::vector<short> loadAllData();

		int getFormat();

		SndfileHandle* handle = nullptr;

		friend class AudioSource;
		friend class MusicSource;
	};
}

