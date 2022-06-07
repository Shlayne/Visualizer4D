#include "Engine/pch.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Application.h"
#if SYSTEM_WINDOWS
	#include "Platform/System/Windows/WindowsInput.h"
#endif

namespace eng
{
	Scope<Input> Input::CreateScope(EventCallback&& rrfEventCallback)
	{
#if SYSTEM_WINDOWS
		return eng::CreateScope<WindowsInput>(std::move(rrfEventCallback));
#endif
	}

	Input& Input::Get()
	{
		return Application::Get().GetInput();
	}
}
