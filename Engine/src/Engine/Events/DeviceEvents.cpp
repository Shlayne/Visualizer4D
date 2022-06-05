#include "Engine/pch.h"
#if ENABLE_LOGGING
#include "Engine/Events/DeviceEvents.h"
#include <sstream>

namespace eng
{
	JoystickConnectEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "JoystickConnectEvent: connected" << m_Connected;
		return stream.str();
	}
}

#endif
