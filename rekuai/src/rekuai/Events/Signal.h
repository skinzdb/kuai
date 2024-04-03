#pragma once

#include <functional>

namespace kuai {
	template<typename R, typename ...Args>
	class Signal
	{
	public:
		Signal() = default;

		void connect()
		{
			
		}

		void collect(const std::function<R>& action)
		{

		}

		void publish(Args&&... args)
		{

		}

	private:
		std::function<R(Args...)> action;
	};
}