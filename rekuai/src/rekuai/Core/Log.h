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

		inline static std::shared_ptr<spdlog::logger>& get_core_logger() { return core_logger; }
		inline static std::shared_ptr<spdlog::logger>& get_client_logger() { return client_logger; }

	private:
		static std::shared_ptr<spdlog::logger> core_logger;
		static std::shared_ptr<spdlog::logger> client_logger;

	};
}

// Core log macros
#define KU_CORE_ERROR(...) ::kuai::Log::get_core_logger()->error(__VA_ARGS__)
#define KU_CORE_WARN(...) ::kuai::Log::get_core_logger()->warn(__VA_ARGS__)
#define KU_CORE_INFO(...) ::kuai::Log::get_core_logger()->info(__VA_ARGS__)
#define KU_CORE_TRACE(...) ::kuai::Log::get_core_logger()->trace(__VA_ARGS__)
#define KU_CORE_CRITICAL(...) ::kuai::Log::get_core_logger()->critical(__VA_ARGS__)

// Client log macros
#define KU_ERROR(...) ::kuai::Log::get_client_logger()->error(__VA_ARGS__)
#define KU_WARN(...) ::kuai::Log::get_client_logger()->warn(__VA_ARGS__)
#define KU_INFO(...) ::kuai::Log::get_client_logger()->info(__VA_ARGS__)
#define KU_TRACE(...) ::kuai::Log::get_client_logger()->trace(__VA_ARGS__)
#define KU_CRITICAL(...) ::kuai::Log::get_client_logger()->critical(__VA_ARGS__)

// @endcond