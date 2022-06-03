#include "Engine/pch.h"
#include "Engine/Core/Window.h"
#if SYSTEM_WINDOWS
	#include "Platform/System/Windows/WindowsWindow.h"
#endif

namespace eng
{
	Ref<Window> Window::CreateRef(const WindowSpecifications& specs, const Ref<Window>& shareContextWindow)
	{
#if SYSTEM_WINDOWS
		return eng::CreateRef<WindowsWindow>(specs, shareContextWindow);
#endif
	}
}
