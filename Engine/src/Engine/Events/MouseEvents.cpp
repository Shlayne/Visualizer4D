#include "Engine/pch.h"
#if ENABLE_LOGGING
#include "Engine/Events/MouseEvents.h"
#include <sstream>

namespace eng
{
	MouseButtonPressEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "MouseButtonPressEvent: button=" << m_Button << " modifiers=" << m_Modifiers;
		return stream.str();
	}

	MouseButtonReleaseEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "MouseButtonReleaseEvent: button=" << m_Button << " modifiers=" << m_Modifiers;
		return stream.str();
	}

	MouseMoveEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "MouseMoveEvent: x=" << m_X << " y=" << m_Y;
		return stream.str();
	}

	MouseScrollEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "MouseScrollEvent: offsetX=" << m_OffsetX << " offsetY=" << m_OffsetY;
		return stream.str();
	}

	MouseEnterEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "MouseEnterEvent: entered=" << m_Entered;
		return stream.str();
	}
}

#endif
