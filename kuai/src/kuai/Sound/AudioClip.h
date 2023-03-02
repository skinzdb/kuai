#pragma once

namespace kuai {
	class AudioClip
	{
	public:
		AudioClip(std::string& filename);
		~AudioClip();

	private:
		uint32_t bufferId = 0;
	};
}

