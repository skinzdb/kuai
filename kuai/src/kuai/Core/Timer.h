#pragma once

#include <chrono>

namespace kuai {
	/** \class Timer
	*	\brief Starts timing upon instantiation. Used to measure elapsed time. 
	*/
	class Timer {
	public:
		Timer() { reset(); }

		/**
		* Returns time elapsed since last call in seconds.
		*/
		float getElapsed()
		{
			auto time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin);
			reset();
			return time.count() * 0.000001f;
		}

		/**
		* Returns time elapsed since last call in milliseconds.
		*/
		float getElaspedMillis()
		{
			return getElapsed() * 1000.0f;
		}
		
		/**
		* Returns time elapsed since last call in microseconds.
		*/
		float getElapsedMicros()
		{
			return getElapsed() * 1000000.0f;
		}

		/**
		* Reset elapsed time to zero.
		*/
		void reset()
		{ 
			begin = std::chrono::steady_clock::now();
		}

	private:
		std::chrono::steady_clock::time_point begin;
	};
}