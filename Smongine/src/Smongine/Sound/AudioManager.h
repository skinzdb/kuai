#pragma once

struct ALCdevice;
struct ALCcontext;

namespace Smong {
	class AudioManager
	{
	public:
		AudioManager();
		~AudioManager();

		uint32_t CreateAudioSource();
		void PlayAudioSource(uint32_t sourceId);

		void Update();

		static AudioManager Get() { return *instance; }
	private:
		bool CheckAlErrors();
		bool CheckAlcErrors(ALCdevice* device);

	private:
		ALCdevice* device;
		ALCcontext* context;

		std::vector<uint32_t> sourceIds;
		static AudioManager* instance;
	};
}


