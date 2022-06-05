#include "Engine/pch.h"
#if ENABLE_LOGGING
#include "Engine/Events/WindowEvents.h"
#include <sstream>

namespace eng
{
	WindowCloseEvent::operator std::string() const
	{
		return "WindowCloseEvent";
	}

	WindowResizeEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "WindowResizeEvent: width=" << m_Width << " height=" << m_Height;
		return stream.str();
	}

	WindowFramebufferResizeEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "WindowFramebufferResizeEvent: width=" << m_Width << " height=" << m_Height;
		return stream.str();
	}

	WindowMoveEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "WindowMoveEvent: x=" << m_X << " y=" << m_Y;
		return stream.str();
	}

	WindowFocusEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "WindowFocusEvent: focused=" << m_Focused;
		return stream.str();
	}

	WindowMinimizeEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "WindowMinimizeEvent: minimized=" << m_Minimized;
		return stream.str();
	}

	WindowMaximizeEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "WindowMaximizeEvent: maximized=" << m_Maximized;
		return stream.str();
	}

	WindowPathDropEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "WindowPathDropEvent: count=" << m_Count << " paths=[\"" << m_ppPaths[0];
		for (sint32 i = 1; i < m_Count; i++)
			stream << "\",\"" << m_ppPaths[i];
		stream << "\"]";
		return stream.str();
	}

	WindowContentScaleEvent::operator std::string() const
	{
		std::stringstream stream;
		stream << "WindowContentScaleEvent: scaleX=" << m_ScaleX << " scaleY=" << m_ScaleY;
		return stream.str();
	}

	WindowRefreshEvent::operator std::string() const
	{
		return "WindowRefreshEvent";
	}
}

#endif
