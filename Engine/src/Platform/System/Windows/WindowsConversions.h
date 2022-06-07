#pragma once

#include "Engine/Core/Keycodes.h"
#include "Engine/Core/MouseButtons.h"
#include "Engine/Core/Modifiers.h"
#include "Engine/Core/Joysticks.h"

namespace eng
{
	// Conversions

	Keycode ConvertKeycode(sint32 keycode);
	MouseButton ConvertMouseButton(sint32 button);
	Modifiers ConvertModifiers(sint32 mods);
	Joystick ConvertJoystickID(sint32 jid);
	JoystickButton ConvertJoystickButton(sint32 button);
	JoystickAxis ConvertJoystickAxis(sint32 axis);
	JoystickHat ConvertJoystickHat(sint32 hat);

	// Unconversions

	sint32 UnconvertJoystickID(Joystick joystick);
}
