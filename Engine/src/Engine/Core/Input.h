#pragma once

#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"

namespace eng
{
	class Input
	{
	private:
		friend class Application;

		static Timestep GetElapsedTime();
		static void SetEventCallback(const eng::EventCallback& crfEventCallback);
		static void PollEvents();
	private:
		friend class Window;
		static void EventCallback(Event& event);
	};
}
