#pragma once

#if ENABLE_ASSERTS
	#define ASSERT_IMPL(type, condition, message, ...) \
		do \
		{ \
			if (!(condition)) \
			{ \
				LOG##type##ERROR("Assertion ({0}) failed at {1}:{2}", (condition), __FILE__, __LINE__); \
				LOG##type##ERROR(message __VA_OPT__(,) __VA_ARGS__); \
				DEBUG_BREAK(); \
			} \
		} \
		while (false)

	#define CORE_ASSERT(condition, message, ...) EXPAND(ASSERT_IMPL(_CORE_, condition, message __VA_OPT__(,) __VA_ARGS__))
	#define ASSERT(condition, message, ...) EXPAND(ASSERT_IMPL(_, condition, message __VA_OPT__(,) __VA_ARGS__))
#else
	#define CORE_ASSERT(...)
	#define ASSERT(...)
#endif
