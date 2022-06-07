#pragma once

#include "Engine/Core/Int.h"

// Taken from glfw3.h and modified.

namespace eng
{
	using Keycode = uint16;
	enum : Keycode
	{
		/* Printable keys */
		Keycode_Space      = 32,
		Keycode_Apostrophe = 39, /* ' */
		Keycode_Comma      = 44, /* , */
		Keycode_Minus      = 45, /* - */
		Keycode_Period     = 46, /* . */
		Keycode_Slash      = 47, /* / */

		Keycode_0 = 48, /* 0 */
		Keycode_1 = 49, /* 1 */
		Keycode_2 = 50, /* 2 */
		Keycode_3 = 51, /* 3 */
		Keycode_4 = 52, /* 4 */
		Keycode_5 = 53, /* 5 */
		Keycode_6 = 54, /* 6 */
		Keycode_7 = 55, /* 7 */
		Keycode_8 = 56, /* 8 */
		Keycode_9 = 57, /* 9 */

		Keycode_Semicolon = 59, /* ; */
		Keycode_Equal     = 61, /* = */

		Keycode_A = 65,
		Keycode_B = 66,
		Keycode_C = 67,
		Keycode_D = 68,
		Keycode_E = 69,
		Keycode_F = 70,
		Keycode_G = 71,
		Keycode_H = 72,
		Keycode_I = 73,
		Keycode_J = 74,
		Keycode_K = 75,
		Keycode_L = 76,
		Keycode_M = 77,
		Keycode_N = 78,
		Keycode_O = 79,
		Keycode_P = 80,
		Keycode_Q = 81,
		Keycode_R = 82,
		Keycode_S = 83,
		Keycode_T = 84,
		Keycode_U = 85,
		Keycode_V = 86,
		Keycode_W = 87,
		Keycode_X = 88,
		Keycode_Y = 89,
		Keycode_Z = 90,

		Keycode_LeftBracket  =  91, /* [ */
		Keycode_BackSlash    =  92, /* \ */
		Keycode_RightBracket =  93, /* ] */
		Keycode_GraveAccent  =  96, /* ` */
		Keycode_World1       = 161, /* non-US #1 */
		Keycode_World2       = 162, /* non-US #2 */

		/* Function keys */
		Keycode_Escape      = 256,
		Keycode_Enter       = 257,
		Keycode_Tab         = 258,
		Keycode_Backspace   = 259,
		Keycode_Insert      = 260,
		Keycode_Delete      = 261,
		Keycode_Right       = 262,
		Keycode_Left        = 263,
		Keycode_Down        = 264,
		Keycode_Up          = 265,
		Keycode_PageUp      = 266,
		Keycode_PageDown    = 267,
		Keycode_Home        = 268,
		Keycode_End         = 269,
		Keycode_CapsLock    = 280,
		Keycode_ScrollLock  = 281,
		Keycode_NumLock     = 282,
		Keycode_PrintScreen = 283,
		Keycode_Pause       = 284,

		Keycode_F1  = 290,
		Keycode_F2  = 291,
		Keycode_F3  = 292,
		Keycode_F4  = 293,
		Keycode_F5  = 294,
		Keycode_F6  = 295,
		Keycode_F7  = 296,
		Keycode_F8  = 297,
		Keycode_F9  = 298,
		Keycode_F10 = 299,
		Keycode_F11 = 300,
		Keycode_F12 = 301,
		Keycode_F13 = 302,
		Keycode_F14 = 303,
		Keycode_F15 = 304,
		Keycode_F16 = 305,
		Keycode_F17 = 306,
		Keycode_F18 = 307,
		Keycode_F19 = 308,
		Keycode_F20 = 309,
		Keycode_F21 = 310,
		Keycode_F22 = 311,
		Keycode_F23 = 312,
		Keycode_F24 = 313,
		Keycode_F25 = 314,

		/* Keypad */
		Keycode_Keypad_0 = 320,
		Keycode_Keypad_1 = 321,
		Keycode_Keypad_2 = 322,
		Keycode_Keypad_3 = 323,
		Keycode_Keypad_4 = 324,
		Keycode_Keypad_5 = 325,
		Keycode_Keypad_6 = 326,
		Keycode_Keypad_7 = 327,
		Keycode_Keypad_8 = 328,
		Keycode_Keypad_9 = 329,

		Keycode_Keypad_Decimal  = 330,
		Keycode_Keypad_Divide   = 331,
		Keycode_Keypad_Multiply = 332,
		Keycode_Keypad_Subtract = 333,
		Keycode_Keypad_Add      = 334,
		Keycode_Keypad_Enter    = 335,
		Keycode_Keypad_Equal    = 336,

		Keycode_LeftShift    = 340,
		Keycode_LeftControl  = 341,
		Keycode_LeftAlt      = 342,
		Keycode_LeftSuper    = 343,
		Keycode_RightShift   = 344,
		Keycode_RightControl = 345,
		Keycode_RightAlt     = 346,
		Keycode_RightSuper   = 347,
		Keycode_Menu         = 348,

		Keycode_Count
	};
}
