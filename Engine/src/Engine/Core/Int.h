#pragma once

#include <cstdint>

namespace eng
{
	using sint08 = int8_t;
	using sint16 = int16_t;
	using sint32 = int32_t;
	using sint64 = int64_t;

	using uint08 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
	using uint64 = uint64_t;

	constexpr inline sint08 sint08_min = static_cast<sint08>(0x80);
	constexpr inline sint16 sint16_min = static_cast<sint16>(0x8000);
	constexpr inline sint32 sint32_min = static_cast<sint32>(0x80000000);
	constexpr inline sint64 sint64_min = static_cast<sint64>(0x8000000000000000);

	constexpr inline sint08 sint08_max = static_cast<sint08>(0x7F);
	constexpr inline sint16 sint16_max = static_cast<sint16>(0x7FFF);
	constexpr inline sint32 sint32_max = static_cast<sint32>(0x7FFFFFFF);
	constexpr inline sint64 sint64_max = static_cast<sint64>(0x7FFFFFFFFFFFFFFF);

	constexpr inline uint08 uint08_max = static_cast<uint08>(0xFF);
	constexpr inline uint16 uint16_max = static_cast<uint16>(0xFFFF);
	constexpr inline uint32 uint32_max = static_cast<uint32>(0xFFFFFFFF);
	constexpr inline uint64 uint64_max = static_cast<uint64>(0xFFFFFFFFFFFFFFFF);
}
