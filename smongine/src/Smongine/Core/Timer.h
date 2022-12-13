#pragma once

#include <chrono>

namespace Smong {
	class Timer {
	public:
		Timer() { Reset(); }

		/**
		* Returns elapsed time since last call in milliseconds
		*/
		uint64_t GetElapsed()
		{
			auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin);
			Reset();
			return time.count();
		}

		void Reset()
		{ 
			begin = std::chrono::steady_clock::now();
		}

	private:
		std::chrono::steady_clock::time_point begin;
	};
}