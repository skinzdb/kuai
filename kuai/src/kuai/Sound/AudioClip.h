#pragma once

namespace kuai {
	class AudioClip
	{
	public:
		AudioClip(const std::string& filename);
		~AudioClip();

	private:
		int format = 0;
		int samplerate = 0;
		std::vector<short> data;

		size_t cursor = 0; // Points to how far through `data` we are, used for streaming

		friend class AudioManager;
	};
}

