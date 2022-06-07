#pragma once

// This should always be included immediately after OpenGLRendererAPIBind.h.

// Client logging functions are not used, as this is never client code.
#undef LOG_TRACE
#undef LOG_DEBUG
#undef LOG_INFO
#undef LOG_WARN
#undef LOG_ERROR
#undef LOG_FATAL

#undef ASSERT

#if ENABLE_LOGGING
	#undef LOG_CORE_TRACE
	#undef LOG_CORE_DEBUG
	#undef LOG_CORE_INFO
	#undef LOG_CORE_WARN
	#undef LOG_CORE_ERROR
	#undef LOG_CORE_FATAL

	#define LOG_CORE_TRACE(...) ::eng::Bind::GetCoreLogger()->trace(__VA_ARGS__)
	#define LOG_CORE_DEBUG(...) ::eng::Bind::GetCoreLogger()->debug(__VA_ARGS__)
	#define LOG_CORE_INFO(...)  ::eng::Bind::GetCoreLogger()->info(__VA_ARGS__)
	#define LOG_CORE_WARN(...)  ::eng::Bind::GetCoreLogger()->warn(__VA_ARGS__)
	#define LOG_CORE_ERROR(...) ::eng::Bind::GetCoreLogger()->error(__VA_ARGS__)
	#define LOG_CORE_FATAL(...) ::eng::Bind::GetCoreLogger()->critical(__VA_ARGS__)
#endif
