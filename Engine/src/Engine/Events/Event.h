#pragma once

#include <functional>
#if ENABLE_LOGGING
	#include <string>
#endif

namespace eng
{
	enum class EventType
	{
		WindowClose, WindowResize, WindowFramebufferResize, WindowMove, WindowFocus,
			WindowMinimize, WindowMaximize, WindowPathDrop, WindowContentScale, WindowRefresh,
		KeyPress, KeyRepeat, KeyRelease, CharType,
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll, MouseEnter,
		JoystickConnect, MonitorConnect
	};

	using EventCategory = uint8;
	enum : EventCategory
	{
		EventCategory_None = 0,
		EventCategory_Window      = 1 << 0,
		EventCategory_Keyboard    = 1 << 1,
		EventCategory_Mouse       = 1 << 2,
		EventCategory_MouseButton = 1 << 3,
		EventCategory_Device      = 1 << 4
	};

#define EVENT_TYPE(type) \
		static constexpr EventType GetStaticType() { return (type); } \
		inline virtual EventType GetType() const override { return GetStaticType(); }

#define EVENT_CATEGORIES(categories) \
		inline virtual EventCategory GetCategories() const override { return (categories); }

	class Event
	{
	public:
		virtual EventType GetType() const = 0;
		virtual EventCategory GetCategories() const = 0;
		inline bool IsInCategories(EventCategory categories) const;
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const = 0;
#endif
	public:
		template<typename C, typename E>
		inline void Dispatch(C* object, void(C::*fCallback)(E&));
		template<typename E>
		inline void Dispatch(void(*fCallback)(E&));
	public:
		inline bool IsHandled() const;
		inline void Handle();
	private:
		bool m_Handled = false;
	};

	using EventCallback = std::function<void(Event&)>;
}

#include "Event.inl"
