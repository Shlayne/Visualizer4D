#pragma once

#if CONFIG_DEBUG
	#if PLATFORM_WINDOWS
		#define DEBUG_BREAK() __debugbreak()
	#endif
#else
	#define DEBUG_BREAK()
#endif
