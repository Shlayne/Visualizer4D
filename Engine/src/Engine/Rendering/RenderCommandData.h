#pragma once

#include "Engine/Core/Int.h"

namespace eng
{
	struct SetViewportData
	{
		constexpr SetViewportData(const glm::s32vec2& position, const glm::s32vec2& size)
			: position(position), size(size) {}

		glm::s32vec2 position;
		glm::s32vec2 size;
	};

	struct SetClearColorData
	{
		constexpr SetClearColorData(const glm::vec4& color)
			: color(color) {}

		glm::vec4 color;
	};

	//struct DrawIndexedData
	//{
	//	// TODO
	//};
}
