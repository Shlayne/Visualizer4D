#pragma once

#include "Engine/Core/Core.h"
#include <spdlog/spdlog.h>

namespace eng
{
	struct CommandLineArgs;

	class Logger
	{
	public:
		static inline Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static inline Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	private:
		friend int Main(CommandLineArgs args);
		static void Init();
	};
}

#if ENABLE_LOGGING
	#define LOG_CORE_TRACE(...) ::eng::Logger::GetCoreLogger()->trace(__VA_ARGS__)
	#define LOG_CORE_DEBUG(...) ::eng::Logger::GetCoreLogger()->debug(__VA_ARGS__)
	#define LOG_CORE_INFO(...)  ::eng::Logger::GetCoreLogger()->info(__VA_ARGS__)
	#define LOG_CORE_WARN(...)  ::eng::Logger::GetCoreLogger()->warn(__VA_ARGS__)
	#define LOG_CORE_ERROR(...) ::eng::Logger::GetCoreLogger()->error(__VA_ARGS__)
	#define LOG_CORE_FATAL(...) ::eng::Logger::GetCoreLogger()->critical(__VA_ARGS__)

	#define LOG_TRACE(...) ::eng::Logger::GetClientLogger()->trace(__VA_ARGS__)
	#define LOG_DEBUG(...) ::eng::Logger::GetClientLogger()->debug(__VA_ARGS__)
	#define LOG_INFO(...)  ::eng::Logger::GetClientLogger()->info(__VA_ARGS__)
	#define LOG_WARN(...)  ::eng::Logger::GetClientLogger()->warn(__VA_ARGS__)
	#define LOG_ERROR(...) ::eng::Logger::GetClientLogger()->error(__VA_ARGS__)
	#define LOG_FATAL(...) ::eng::Logger::GetClientLogger()->critical(__VA_ARGS__)
#else
	#define LOG_CORE_TRACE(...)
	#define LOG_CORE_DEBUG(...)
	#define LOG_CORE_INFO(...)
	#define LOG_CORE_WARN(...)
	#define LOG_CORE_ERROR(...)
	#define LOG_CORE_FATAL(...)

	#define LOG_TRACE(...)
	#define LOG_DEBUG(...)
	#define LOG_INFO(...)
	#define LOG_WARN(...)
	#define LOG_ERROR(...)
	#define LOG_FATAL(...)
#endif