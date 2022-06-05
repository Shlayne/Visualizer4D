#include "Engine/pch.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/Input.h"
#if SYSTEM_WINDOWS
	#include "Platform/System/Windows/WindowsWindow.h"
#endif

namespace eng
{
	Ref<Window> Window::CreateRef(const WindowSpecifications& crSpecs, const Ref<Window>& crShareContextWindow)
	{
#if SYSTEM_WINDOWS
		return eng::CreateRef<WindowsWindow>(crSpecs, crShareContextWindow);
#endif
	}

	void Window::EventCallback(Event& event)
	{
		Input::EventCallback(event);
	}
}
