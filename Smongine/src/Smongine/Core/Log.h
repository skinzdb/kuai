#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Smong {
	class Log
	{
	public:
		Log();
		~Log();

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;

	};
}

// Core log macros
#define SM_CORE_ERROR(...) ::Smong::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SM_CORE_WARN(...) ::Smong::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SM_CORE_INFO(...) ::Smong::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SM_CORE_TRACE(...) ::Smong::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SM_CORE_CRITICAL(...) ::Smong::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SM_ERROR(...) ::Smong::Log::GetClientLogger()->error(__VA_ARGS__)
#define SM_WARN(...) ::Smong::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SM_INFO(...) ::Smong::Log::GetClientLogger()->info(__VA_ARGS__)
#define SM_TRACE(...) ::Smong::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SM_CRITICAL(...) ::Smong::Log::GetClientLogger()->critical(__VA_ARGS__)