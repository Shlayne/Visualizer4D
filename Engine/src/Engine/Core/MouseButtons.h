#pragma once

#include "Engine/Core/Int.h"

// Taken from glfw3.h and modified.

namespace eng
{
	enum class MouseButton : uint8
	{
		Button1 = 0, ButtonLeft     = Button1,
		Button2 = 1, ButtonRight    = Button2,
		Button3 = 2, ButtonMiddle   = Button3,
		Button4 = 3, ButtonBackward = Button4,
		Button5 = 4, ButtonForward  = Button5,
		Button6 = 5,
		Button7 = 6,
		Button8 = 7,

		Count
	};
}

#if ENABLE_LOGGING
#include <ostream>

template <class _Elem, class _Traits = std::char_traits<_Elem>>
std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& ostream, eng::MouseButton button)
{
	return ostream << +static_cast<eng::uint8>(button);
}
#endif

