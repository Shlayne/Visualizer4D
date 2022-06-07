#include "Engine/pch.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/Input.h"
#if SYSTEM_WINDOWS
	#include "Platform/System/Windows/WindowsWindow.h"
#endif

namespace eng
{
	Scope<Window> Window::CreateScope(const WindowSpecifications& crSpecs, const Scope<Window>& crShareContextWindow)
	{
#if SYSTEM_WINDOWS
		return eng::CreateScope<WindowsWindow>(crSpecs, crShareContextWindow);
#endif
	}

	void Window::OnEvent(Event& rEvent)
	{
		Input::Get().OnEvent(rEvent);
	}
}
