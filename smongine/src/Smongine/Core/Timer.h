#pragma once

#include <chrono>

namespace Smong {
	class Timer {
	public:
		Timer() { Reset(); }

		/**
		* Returns elapsed time since last call in seconds
		*/
		float GetElapsed()
		{
			auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin);
			Reset();
			return time.count() * 0.001f;
		}

		float GetElaspedMillis()
		{
			return GetElapsed() * 1000.0f;
		}

		void Reset()
		{ 
			begin = std::chrono::steady_clock::now();
		}

	private:
		std::chrono::steady_clock::time_point begin;
	};
}