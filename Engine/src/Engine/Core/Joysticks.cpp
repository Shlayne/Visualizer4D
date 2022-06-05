#include "Engine/pch.h"
#include "Engine/Core/Joysticks.h"

namespace eng
{
	JoystickState::~JoystickState()
	{
		if (m_Connected)
			Disconnect();
	}

	void JoystickState::Connect(uint32 buttonCount, uint32 axisCount, uint32 hatCount)
	{
		CORE_ASSERT(!m_Connected, "Attempted to reconnect joystick!");
		CORE_ASSERT(buttonCount <= JoystickButton_Count, "Joystick has too many buttons!");
		CORE_ASSERT(axisCount <= JoystickAxis_Count, "Joystick has too many axes!");
		CORE_ASSERT(hatCount <= JoystickHat_Count, "Joystick has too many hats!");

		m_Connected = true;

		m_ButtonCount = buttonCount;
		m_AxisCount = axisCount;
		m_HatCount = hatCount;

		m_pButtons = buttonCount != 0 ? new uint8[1 + (buttonCount - 1) / 8]() : nullptr;
		m_pAxes = axisCount != 0 ? new float[axisCount]() : nullptr;
		m_pHats = hatCount != 0 ? new JoystickHatState[1 + (hatCount - 1) / 2]() : nullptr;
	}

	void JoystickState::Disconnect()
	{
		CORE_ASSERT(m_Connected, "Attempted to redisconnect joystick!");

		m_Connected = false;

		m_ButtonCount = 0;
		m_AxisCount = 0;
		m_HatCount = 0;

		delete[] m_pButtons;
		delete[] m_pAxes;
		delete[] m_pHats;
	}

	bool JoystickState::GetButton(JoystickButton button) const
	{
		CORE_ASSERT(m_Connected, "Attempted to read button status from disconnected joystick!");
		CORE_ASSERT(button < m_ButtonCount, "Joystick button index out of bounds!");
		return m_pButtons[button / 8] & (1 << (button % 8));
	}

	float JoystickState::GetAxis(JoystickAxis axis) const
	{
		CORE_ASSERT(m_Connected, "Attempted to read axis status from disconnected joystick!");
		CORE_ASSERT(axis < m_AxisCount, "Joystick axis index out of bounds!");
		return m_pAxes[axis];
	}

	JoystickHatState JoystickState::GetHat(JoystickHat hat) const
	{
		CORE_ASSERT(m_Connected, "Attempted to read hat status from disconnected joystick!");
		CORE_ASSERT(hat < m_HatCount, "Joystick hat index out of bounds!");
		return m_pHats[hat / 2] & (0xF << (hat % 2));
	}

	void JoystickState::SetButton(JoystickButton button, bool value)
	{
		CORE_ASSERT(m_Connected, "Attempted to write button status to disconnected joystick!");
		CORE_ASSERT(button < m_ButtonCount, "Joystick button index out of bounds!");

		uint8 bit = 1 << (button % 8);
		uint8 previousState = m_pButtons[button / 8];
		m_pButtons[button / 8] = value ? previousState | bit : previousState & ~bit;
	}

	void JoystickState::SetAxis(JoystickAxis axis, float value)
	{
		CORE_ASSERT(m_Connected, "Attempted to write axis status to disconnected joystick!");
		CORE_ASSERT(axis < m_AxisCount, "Joystick axis index out of bounds!");

		m_pAxes[axis] = value;
	}

	void JoystickState::SetHat(JoystickHat hat, JoystickHatState value)
	{
		CORE_ASSERT(m_Connected, "Attempted to write hat status to disconnected joystick!");
		CORE_ASSERT(hat < m_HatCount, "Joystick hat index out of bounds!");
		CORE_ASSERT((value & 0xF) == value, "Attempted to set a hat to an invalid state.");

		uint8 bits = 4 * (hat % 2);
		m_pHats[hat / 2] = (m_pHats[hat / 2] & ~(0xF << bits)) | (value << bits);
	}
}
