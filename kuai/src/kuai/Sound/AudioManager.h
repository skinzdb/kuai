#pragma once

struct ALCdevice;
struct ALCcontext;

namespace kuai {
	class AudioManager
	{
	public:
		static void init();
		static void cleanup();

		static uint32_t createAudioSource();
		static void playAudioSource(uint32_t sourceId);

		static void update();
	private:
		static bool checkAlErrors();
		static bool checkAlcErrors(ALCdevice* device);

	private:
		static ALCdevice* device;
		static ALCcontext* context;

		static std::vector<uint32_t> sourceIds;
	};
}


