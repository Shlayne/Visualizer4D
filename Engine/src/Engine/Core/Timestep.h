#pragma once

namespace eng
{
	struct Timestep
	{
	public:
		constexpr Timestep() noexcept = default;
		constexpr Timestep(float seconds) noexcept : m_Seconds(seconds) {}
		constexpr Timestep(const Timestep& timestep) noexcept : m_Seconds(timestep.m_Seconds) {}
		constexpr Timestep& operator=(float seconds) noexcept { m_Seconds = seconds; return *this; }
		constexpr Timestep& operator=(const Timestep& timestep) noexcept { m_Seconds = timestep.m_Seconds; return *this; }
		constexpr operator float() const noexcept { return m_Seconds; }
	public:
		constexpr float Seconds() const noexcept { return m_Seconds; }
		constexpr float Millis() const noexcept { return m_Seconds * 1000.0f; }
		constexpr float Micros() const noexcept { return m_Seconds * 1000000.0f; }
		constexpr float Nanos() const noexcept { return m_Seconds * 1000000000.0f; }
	public:
		constexpr Timestep  operator+ (Timestep timestep) const noexcept { return m_Seconds + timestep.m_Seconds; }
		constexpr Timestep& operator+=(Timestep timestep) noexcept { m_Seconds += timestep.m_Seconds; return *this; }
		constexpr Timestep  operator- (Timestep timestep) const noexcept { return m_Seconds - timestep.m_Seconds; }
		constexpr Timestep& operator-=(Timestep timestep) noexcept { m_Seconds -= timestep.m_Seconds; return *this; }
		constexpr Timestep  operator* (Timestep timestep) const noexcept { return m_Seconds * timestep.m_Seconds; }
		constexpr Timestep& operator*=(Timestep timestep) noexcept { m_Seconds *= timestep.m_Seconds; return *this; }
		constexpr Timestep  operator/ (Timestep timestep) const noexcept { return m_Seconds / timestep.m_Seconds; }
		constexpr Timestep& operator/=(Timestep timestep) noexcept { m_Seconds /= timestep.m_Seconds; return *this; }
	private:
		float m_Seconds = 0.0f;
	};
}
