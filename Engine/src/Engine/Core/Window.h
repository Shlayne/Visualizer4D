#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Rendering/Context.h"
#include <glm/glm.hpp>
#include <string_view>

namespace eng
{
	struct WindowSpecifications
	{
		sint32 width = 1280;
		sint32 height = 720;
		std::string_view title = "Untitled Window";
	};

	class Window
	{
	public:
		static Ref<Window> CreateRef(const WindowSpecifications& crSpecs, const Ref<Window>& crShareContextWindow);
		virtual ~Window() = default;
	public:
		virtual const Ref<Context>& GetContext() const = 0;
		//virtual sint32 GetContentAreaWidth() const = 0;
		//virtual sint32 GetContentAreaHeight() const = 0;
		//virtual glm::s32vec2 GetContentAreaSize() const = 0;
	private:
		friend class Application;
		static void PollEvents();
	};
}
