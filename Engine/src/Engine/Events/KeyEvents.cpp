#include "Engine/pch.h"
#if ENABLE_LOGGING
#include "Engine/Events/KeyEvents.h"
#include <sstream>

namespace eng
{
	KeyPressEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "KeyPressEvent: keycode=" << m_Keycode << " modifiers=" << m_Modifiers;
		return stream.str();
	}

	KeyRepeatEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "KeyRepeatEvent: keycode=" << m_Keycode << " modifiers=" << m_Modifiers;
		return stream.str();
	}

	KeyReleaseEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "KeyReleaseEvent: keycode=" << m_Keycode << " modifiers=" << m_Modifiers;
		return stream.str();
	}

	CharTypeEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "KeyCharEvent: codepoint=" << m_Codepoint;
		return stream.str();
	}
}

#endif
