#pragma once

#include "Engine/Events/Event.h"

namespace eng
{
	class WindowEvent : public Event
	{
	public:
		WindowEvent(void* pNativeWindow)
			: m_pNativeWindow(pNativeWindow) {}
		EVENT_CATEGORIES(EventCategory_Window)
	public:
		inline void* GetNativeWindow() const { return m_pNativeWindow; }
	private:
		void* m_pNativeWindow;
	};

	class WindowCloseEvent : public WindowEvent
	{
	public:
		WindowCloseEvent(void* pNativeWindow)
			: WindowEvent(pNativeWindow) {}
		EVENT_TYPE(EventType::WindowClose)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	};

	class WindowResizeEvent : public WindowEvent
	{
	public:
		WindowResizeEvent(void* pNativeWindow, sint32 width, sint32 height)
			: WindowEvent(pNativeWindow), m_Width(width), m_Height(height) {}
		EVENT_TYPE(EventType::WindowResize)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline sint32 GetWidth() const { return m_Width; }
		inline sint32 GetHeight() const { return m_Height; }
	private:
		sint32 m_Width;
		sint32 m_Height;
	};

	class WindowFramebufferResizeEvent : public WindowEvent
	{
	public:
		WindowFramebufferResizeEvent(void* pNativeWindow, sint32 width, sint32 height)
			: WindowEvent(pNativeWindow), m_Width(width), m_Height(height) {}
		EVENT_TYPE(EventType::WindowFramebufferResize)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline sint32 GetWidth() const { return m_Width; }
		inline sint32 GetHeight() const { return m_Height; }
	private:
		sint32 m_Width;
		sint32 m_Height;
	};

	class WindowMoveEvent : public WindowEvent
	{
	public:
		WindowMoveEvent(void* pNativeWindow, sint32 x, sint32 y)
			: WindowEvent(pNativeWindow), m_X(x), m_Y(y) {}
		EVENT_TYPE(EventType::WindowMove)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline sint32 GetX() const { return m_X; }
		inline sint32 GetY() const { return m_Y; }
	private:
		sint32 m_X;
		sint32 m_Y;
	};

	class WindowFocusEvent : public WindowEvent
	{
	public:
		WindowFocusEvent(void* pNativeWindow, bool focused)
			: WindowEvent(pNativeWindow), m_Focused(focused) {}
		EVENT_TYPE(EventType::WindowFocus)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline bool IsFocused() const { return m_Focused; }
	private:
		bool m_Focused;
	};

	class WindowMinimizeEvent : public WindowEvent
	{
	public:
		WindowMinimizeEvent(void* pNativeWindow, bool minimized)
			: WindowEvent(pNativeWindow), m_Minimized(minimized) {}
		EVENT_TYPE(EventType::WindowMinimize)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline bool IsMinimized() const { return m_Minimized; }
	private:
		bool m_Minimized;
	};

	class WindowMaximizeEvent : public WindowEvent
	{
	public:
		WindowMaximizeEvent(void* pNativeWindow, bool maximized)
			: WindowEvent(pNativeWindow), m_Maximized(maximized) {}
		EVENT_TYPE(EventType::WindowMaximize)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline bool IsMaximized() const { return m_Maximized; }
	private:
		bool m_Maximized;
	};

	class WindowPathDropEvent : public WindowEvent
	{
	public:
		WindowPathDropEvent(void* pNativeWindow, sint32 count, const char** ppPaths)
			: WindowEvent(pNativeWindow), m_Count(count), m_ppPaths(ppPaths) {}
		EVENT_TYPE(EventType::WindowPathDrop)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline sint32 GetCount() const { return m_Count; }
		inline const char* const* const GetPaths() const { return m_ppPaths; }
	private:
		sint32 m_Count;
		const char** m_ppPaths;
	};

	class WindowContentScaleEvent : public WindowEvent
	{
	public:
		WindowContentScaleEvent(void* pNativeWindow, float scaleX, float scaleY)
			: WindowEvent(pNativeWindow), m_ScaleX(scaleX), m_ScaleY(scaleY) {}
		EVENT_TYPE(EventType::WindowContentScale)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline float GetScaleX() const { return m_ScaleX; }
		inline float GetScaleY() const { return m_ScaleY; }
	private:
		float m_ScaleX;
		float m_ScaleY;
	};

	class WindowRefreshEvent : public WindowEvent
	{
	public:
		WindowRefreshEvent(void* pNativeWindow)
			: WindowEvent(pNativeWindow) {}
		EVENT_TYPE(EventType::WindowRefresh)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	};
}
