#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Core/Keycodes.h"
#include "Engine/Core/Modifiers.h"

namespace eng
{
	class KeyEvent : public Event
	{
	public:
		KeyEvent(void* pNativeWindow)
			: m_pNativeWindow(pNativeWindow) {}
		EVENT_CATEGORIES(EventCategory_Keyboard)
	public:
		inline void* GetNativeWindow() const { return m_pNativeWindow; }
	private:
		void* m_pNativeWindow;
	};

	class KeyPressEvent : public KeyEvent
	{
	public:
		KeyPressEvent(void* pNativeWindow, Keycode keycode, Modifiers modifiers)
			: KeyEvent(pNativeWindow), m_Keycode(keycode), m_Modifiers(modifiers) {}
		EVENT_TYPE(EventType::KeyPress)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline Keycode GetKeycode() const { return m_Keycode; }
		inline Modifiers GetModifiers() const { return m_Modifiers; }
	private:
		Keycode m_Keycode;
		Modifiers m_Modifiers;
	};

	class KeyRepeatEvent : public KeyEvent
	{
	public:
		KeyRepeatEvent(void* pNativeWindow, Keycode keycode, Modifiers modifiers)
			: KeyEvent(pNativeWindow), m_Keycode(keycode), m_Modifiers(modifiers) {}
		EVENT_TYPE(EventType::KeyRepeat)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline Keycode GetKeycode() const { return m_Keycode; }
		inline Modifiers GetModifiers() const { return m_Modifiers; }
	private:
		Keycode m_Keycode;
		Modifiers m_Modifiers;
	};

	class KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(void* pNativeWindow, Keycode keycode, Modifiers modifiers)
			: KeyEvent(pNativeWindow), m_Keycode(keycode), m_Modifiers(modifiers) {}
		EVENT_TYPE(EventType::KeyRelease)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline Keycode GetKeycode() const { return m_Keycode; }
		inline Modifiers GetModifiers() const { return m_Modifiers; }
	private:
		Keycode m_Keycode;
		Modifiers m_Modifiers;
	};

	class CharTypeEvent : public KeyEvent
	{
	public:
		CharTypeEvent(void* pNativeWindow, uint32 codepoint)
			: KeyEvent(pNativeWindow), m_Codepoint(codepoint) {}
		EVENT_TYPE(EventType::CharType)
#if ENABLE_LOGGING
	public:
		virtual operator std::string() const override;
#endif
	public:
		inline uint32 GetCodepoint() const { return m_Codepoint; }
	private:
		uint32 m_Codepoint;
	};
}
