#pragma once

#include <chrono>

namespace kuai {
	class Timer {
	public:
		Timer() { reset(); }

		/**
		* Returns elapsed time since last call in seconds
		*/
		float getElapsed()
		{
			auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin);
			reset();
			return time.count() * 0.001f;
		}

		float getElaspedMillis()
		{
			return getElapsed() * 1000.0f;
		}

		void reset()
		{ 
			begin = std::chrono::steady_clock::now();
		}

	private:
		std::chrono::steady_clock::time_point begin;
	};
}