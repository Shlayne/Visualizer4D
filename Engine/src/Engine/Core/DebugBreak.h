#pragma once

#if CONFIG_DEBUG
	#if SYSTEM_WINDOWS
		#define DEBUG_BREAK() __debugbreak()
	#endif
#else
	#define DEBUG_BREAK()
#endif
