#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

// @cond
namespace kuai {
	class Log
	{
	public:
		Log();
		~Log();

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;

	};
}

// Core log macros
#define KU_CORE_ERROR(...) ::kuai::Log::getCoreLogger()->error(__VA_ARGS__)
#define KU_CORE_WARN(...) ::kuai::Log::getCoreLogger()->warn(__VA_ARGS__)
#define KU_CORE_INFO(...) ::kuai::Log::getCoreLogger()->info(__VA_ARGS__)
#define KU_CORE_TRACE(...) ::kuai::Log::getCoreLogger()->trace(__VA_ARGS__)
#define KU_CORE_CRITICAL(...) ::kuai::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define KU_ERROR(...) ::kuai::Log::getClientLogger()->error(__VA_ARGS__)
#define KU_WARN(...) ::kuai::Log::getClientLogger()->warn(__VA_ARGS__)
#define KU_INFO(...) ::kuai::Log::getClientLogger()->info(__VA_ARGS__)
#define KU_TRACE(...) ::kuai::Log::getClientLogger()->trace(__VA_ARGS__)
#define KU_CRITICAL(...) ::kuai::Log::getClientLogger()->critical(__VA_ARGS__)

// @endcond