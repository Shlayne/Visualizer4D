#pragma once

#include "Engine/Core/Int.h"

// Taken from glfw3.h and modified.

namespace eng
{
	using Joystick = uint8;
	enum : Joystick
	{
		Joystick_1 = 0,
		Joystick_2 = 1,
		Joystick_3 = 2,
		Joystick_4 = 3,
		Joystick_5 = 4,
		Joystick_6 = 5,
		Joystick_7 = 6,
		Joystick_8 = 7,
		Joystick_9 = 8,
		Joystick_10 = 9,
		Joystick_11 = 10,
		Joystick_12 = 11,
		Joystick_13 = 12,
		Joystick_14 = 13,
		Joystick_15 = 14,
		Joystick_16 = 15,

		Joystick_Count
	};

	using JoystickButton = uint8;
	enum : JoystickButton
	{
		JoystickButton_A            =  0, JoystickButton_Cross    = JoystickButton_A,
		JoystickButton_B            =  1, JoystickButton_Circle   = JoystickButton_B,
		JoystickButton_X            =  2, JoystickButton_Square   = JoystickButton_X,
		JoystickButton_Y            =  3, JoystickButton_Triangle = JoystickButton_Y,
		JoystickButton_LeftBumper   =  4,
		JoystickButton_RightBumper  =  5,
		JoystickButton_Back         =  6, JoystickButton_Share    = JoystickButton_Back,
		JoystickButton_Start        =  7, JoystickButton_Options  = JoystickButton_Start,
		JoystickButton_Guide        =  8, JoystickButton_PSButton = JoystickButton_Guide,
		JoystickButton_LeftThumb    =  9,
		JoystickButton_RightThumb   = 10,
		JoystickButton_DPadUp       = 11,
		JoystickButton_DPadRight    = 12,
		JoystickButton_DPadDown     = 13,
		JoystickButton_DPadLeft     = 14,

		JoystickButton_Count
	};

	using JoystickAxis = uint8;
	enum : JoystickAxis
	{
		JoystickAxis_LeftX        = 0,
		JoystickAxis_LeftY        = 1,
		JoystickAxis_RightX       = 2,
		JoystickAxis_RightY       = 3,
		JoystickAxis_LeftTrigger  = 4,
		JoystickAxis_RightTrigger = 5,

		JoystickAxis_Count
	};

	using JoystickHat = uint8;
	enum : JoystickHat
	{
		JoystickHat_1 = 0,
		JoystickHat_2 = 1,
		JoystickHat_3 = 2,
		JoystickHat_4 = 3,

		JoystickHat_Count
	};

	using JoystickHatState = uint8;
	enum : JoystickHatState
	{
		JoystickHatState_Centered  = 0,
		JoystickHatState_Up        = 1,
		JoystickHatState_Right     = 2,
		JoystickHatState_Down      = 4,
		JoystickHatState_Left      = 8,
		JoystickHatState_RightUp   = JoystickHatState_Right | JoystickHatState_Up,
		JoystickHatState_RightDown = JoystickHatState_Right | JoystickHatState_Down,
		JoystickHatState_LeftUp    = JoystickHatState_Left  | JoystickHatState_Up,
		JoystickHatState_LeftDown  = JoystickHatState_Left  | JoystickHatState_Down
	};

	class JoystickState
	{
	public:
		~JoystickState();
	public:
		inline bool IsConnected() const { return m_Connected; }
		inline uint32 GetButtonCount() const { return m_ButtonCount; }
		inline uint32 GetAxisCount() const { return m_AxisCount; }
		inline uint32 GetHatCount() const { return m_HatCount; }
		bool GetButton(JoystickButton button) const;
		float GetAxis(JoystickAxis axis) const;
		JoystickHatState GetHat(JoystickHat hat) const;
	private:
		friend class Input;
		void Connect(uint32 buttonCount, uint32 axisCount, uint32 hatCount);
		void Disconnect();
		void SetButton(JoystickButton button, bool value);
		void SetAxis(JoystickAxis axis, float value);
		void SetHat(JoystickHat hat, JoystickHatState value);
	private:
		bool m_Connected = false;
		uint32 m_ButtonCount = 0;
		uint32 m_AxisCount = 0;
		uint32 m_HatCount = 0;
		uint8* m_pButtons;
		float* m_pAxes;
		JoystickHatState* m_pHats;
	};
}
