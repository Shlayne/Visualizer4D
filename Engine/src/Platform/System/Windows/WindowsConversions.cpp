#include "Engine/pch.h"
#include "Platform/System/Windows/WindowsConversions.h"
//#define GLFW_INCLUDE_NONE
//#include <glfw/glfw3.h>

namespace eng
{
	// Conversions

	Keycode ConvertKeycode(sint32 key)
	{
		return static_cast<Keycode>(key);
	}

	MouseButton ConvertMouseButton(sint32 button)
	{
		return static_cast<MouseButton>(button);
	}

	Modifiers ConvertModifiers(sint32 mods)
	{
		return static_cast<ModifierFlags>(mods);
		//return (modifiers & GLFW_MOD_SHIFT     ? Modifiers_Shift    : Modifiers_None) |
		//	   (modifiers & GLFW_MOD_CONTROL   ? Modifiers_Control  : Modifiers_None) |
		//	   (modifiers & GLFW_MOD_ALT       ? Modifiers_Alt      : Modifiers_None) |
		//	   (modifiers & GLFW_MOD_SUPER     ? Modifiers_Super    : Modifiers_None) |
		//	   (modifiers & GLFW_MOD_CAPS_LOCK ? Modifiers_CapsLock : Modifiers_None) |
		//	   (modifiers & GLFW_MOD_NUM_LOCK  ? Modifiers_NumLock  : Modifiers_None);
	}

	Joystick ConvertJoystickID(sint32 jid)
	{
		return static_cast<Joystick>(jid);
	}

	JoystickButton ConvertJoystickButton(sint32 button)
	{
		return static_cast<JoystickButton>(button);
	}

	JoystickAxis ConvertJoystickAxis(sint32 axis)
	{
		return static_cast<JoystickAxis>(axis);
	}

	JoystickHat ConvertJoystickHat(sint32 hat)
	{
		return static_cast<JoystickHat>(hat);
	}

	// Unconversions

	sint32 UnconvertJoystickID(Joystick joystick)
	{
		return static_cast<sint32>(joystick);
	}
}
