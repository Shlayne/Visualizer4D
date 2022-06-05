#pragma once

#include "Engine/Core/Int.h"

namespace eng
{
	using ModifierFlags = uint8;
	enum : ModifierFlags
	{
		Modifiers_None = 0,
		Modifiers_Shift    = 1 << 0,
		Modifiers_Control  = 1 << 1,
		Modifiers_Alt      = 1 << 2,
		Modifiers_Super    = 1 << 3,
		Modifiers_CapsLock = 1 << 4,
		Modifiers_NumLock  = 1 << 5,

		// Automatically creates a mask of all existing modifier flags.
		Modifiers_Last = Modifiers_NumLock,
		Modifiers_Mask = (Modifiers_Last << 1) - 1,
		Modifiers_All = Modifiers_Mask // Just an alias.
	};

	class Modifiers
	{
	public:
		constexpr Modifiers() = default;
		constexpr Modifiers(ModifierFlags flags) : m_Flags(flags & Modifiers_Mask) {}
	public:
		constexpr bool HasAnyOf(ModifierFlags flags) const { return !!(m_Flags & flags); }
		constexpr bool operator==(const Modifiers& crModifiers) const { return m_Flags == crModifiers.m_Flags; }
		constexpr operator ModifierFlags() const { return m_Flags; }
	private:
		ModifierFlags m_Flags = Modifiers_None;
	};
}

#if ENABLE_LOGGING
#include <ostream>

template <class _Elem, class _Traits = std::char_traits<_Elem>>
std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& ostream, eng::Modifiers modifiers)
{
	return ostream << +static_cast<eng::ModifierFlags>(modifiers);
}
#endif
