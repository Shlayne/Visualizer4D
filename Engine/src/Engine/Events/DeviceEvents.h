#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Core/Joysticks.h"

namespace eng
{
	class JoystickConnectEvent : public Event
	{
	public:
		JoystickConnectEvent(Joystick joystick, bool connected)
			: m_Joystick(joystick), m_Connected(connected) {}
		EVENT_TYPE(EventType::JoystickConnect)
		EVENT_CATEGORIES(EventCategory_Device)
#if ENABLE_LOGGING
		virtual operator std::string() const override;
#endif
	public:
		inline Joystick GetJoystick() const { return m_Joystick; }
		inline Joystick IsConnected() const { return m_Connected; }
	private:
		Joystick m_Joystick;
		bool m_Connected;
	};
}
