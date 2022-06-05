#include "Engine/pch.h"
#include "Engine/Core/Input.h"
#include <glfw/glfw3.h>

namespace eng
{
	static EventCallback s_fEventCallback = nullptr;

	Timestep Input::GetElapsedTime()
	{
		static float s_LastTime = 0.0f;
		float time = static_cast<float>(glfwGetTime());
		Timestep timestep = time - s_LastTime;
		s_LastTime = time;
		return timestep;
	}

	void Input::SetEventCallback(const eng::EventCallback& crfEventCallback)
	{
		s_fEventCallback = crfEventCallback;
	}

	void Input::PollEvents()
	{
		glfwPollEvents();
	}

	void Input::EventCallback(Event& event)
	{
		s_fEventCallback(event);
	}
}
