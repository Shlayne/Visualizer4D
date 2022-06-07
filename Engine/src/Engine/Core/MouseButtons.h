#pragma once

#include "Engine/Core/Int.h"

// Taken from glfw3.h and modified.

namespace eng
{
	using MouseButton = uint8;
	enum : MouseButton
	{
		MouseButton_1 = 0, MouseButton_Left     = MouseButton_1,
		MouseButton_2 = 1, MouseButton_Right    = MouseButton_2,
		MouseButton_3 = 2, MouseButton_Middle   = MouseButton_3,
		MouseButton_4 = 3, MouseButton_Backward = MouseButton_4,
		MouseButton_5 = 4, MouseButton_Forward  = MouseButton_5,
		MouseButton_6 = 5,
		MouseButton_7 = 6,
		MouseButton_8 = 7,

		MouseButton_Count
	};
}
