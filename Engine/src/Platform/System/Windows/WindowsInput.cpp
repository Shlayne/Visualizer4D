#include "Engine/pch.h"
#include "Platform/System/Windows/WindowsInput.h"
#include "Engine/Events/DeviceEvents.h"
#include "Platform/System/Windows/WindowsConversions.h"
#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

namespace eng
{
	WindowsInput::WindowsInput(eng::EventCallback&& rrfEventCallback)
		: m_fEventCallback(std::move(rrfEventCallback))
	{
		PROFILE_FUNCTION();

#if ENABLE_ASSERTS
		UNUSED(glfwSetErrorCallback([](int errorCode, const char* pDescription)
		{
			CORE_ASSERT(false, "GLFW Error ({0}): {1}", errorCode, pDescription);
		}));
#endif

		glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
		int status = glfwInit();
		CORE_ASSERT(status == GLFW_TRUE, "Failed to initialize GLFW!");

		UNUSED(glfwSetJoystickCallback([](sint32 jid, sint32 event)
		{
			WindowsInput& rInput = *static_cast<WindowsInput*>(&Get());
			switch (event)
			{
				case GLFW_CONNECTED: rInput.OnJoystickConnected(ConvertJoystickID(jid)); break;
				case GLFW_DISCONNECTED: rInput.OnJoystickDisconnected(ConvertJoystickID(jid)); break;
			}
		}));

		// Now that the joystick callback is set, emit some fake connect events
		// for joysticks that were connected prior to running the application.
		for (sint32 jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
			if (glfwJoystickPresent(jid) == GLFW_TRUE)
				OnJoystickConnected(ConvertJoystickID(jid));
	}

	WindowsInput::~WindowsInput()
	{
		UNUSED(glfwSetErrorCallback(NULL));
		glfwTerminate();

		// Reset all static data.

		for (Joystick joystick = Joystick_1; joystick < Joystick_Count; joystick++)
			OnJoystickDisconnected(joystick);
	}

	bool WindowsInput::IsKeyPressed(Keycode keycode) const
	{
		CORE_ASSERT(keycode < Keycode_Count, "Keycode index={0} out of bounds!", keycode);
		return !!(m_Keys[keycode / 8] & (1 << (keycode % 8)));
	}

	bool WindowsInput::IsMouseButtonPressed(MouseButton button) const
	{
		CORE_ASSERT(button < MouseButton_Count, "Mouse Button index={0} out of bounds!", button);
		return !!(m_MouseButtons[button / 8] & (1 << (button % 8)));
	}

	glm::vec2 WindowsInput::GetAbsoluteMousePosition() const
	{
		GLFWwindow* pWindow = glfwGetCurrentContext();
		CORE_ASSERT(pWindow != NULL, "Window was null!");
		glm::s32vec2 windowPos{ 0 };
		glfwGetWindowPos(pWindow, &windowPos.x, &windowPos.y);
		glm::dvec2 cursorPos{ 0.0 };
		glfwGetCursorPos(pWindow, &cursorPos.x, &cursorPos.y);
		return glm::vec2(glm::dvec2(windowPos) + cursorPos);
	}

	glm::vec2 WindowsInput::GetRelativeMousePosition(const void* pNativeWindow) const
	{
		GLFWwindow* pWindow = const_cast<GLFWwindow*>(static_cast<const GLFWwindow*>(pNativeWindow));
		CORE_ASSERT(pWindow != NULL, "Window was null!");
		glm::dvec2 cursorPos{ 0.0 };
		glfwGetCursorPos(pWindow, &cursorPos.x, &cursorPos.y);
		return glm::vec2(cursorPos);
	}

	glm::vec2 WindowsInput::GetRelativeMousePosition(const Window& crWindow) const
	{
		return GetRelativeMousePosition(crWindow.GetNativeWindow());
	}

	bool WindowsInput::IsJoystickConnected(Joystick joystick) const
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index={0} out of bounds!", joystick);
		return m_Joysticks[joystick].IsConnected();
	}

	bool WindowsInput::IsJoystickButtonPressed(Joystick joystick, JoystickButton button) const
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index={0} out of bounds!", joystick);
		const JoystickState& joystickState = m_Joysticks[joystick];
		return joystickState.IsConnected() && joystickState.GetButton(button);
	}

	float WindowsInput::GetJoystickAxis(Joystick joystick, JoystickAxis axis) const
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index={0} out of bounds!", joystick);
		const JoystickState& joystickState = m_Joysticks[joystick];
		return joystickState.IsConnected() ? joystickState.GetAxis(axis) : 0.0f;
	}

	JoystickHatState WindowsInput::GetJoystickHat(Joystick joystick, JoystickHat hat) const
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index={0} out of bounds!", joystick);
		const JoystickState& joystickState = m_Joysticks[joystick];
		return joystickState.IsConnected() ? joystickState.GetHat(hat) : JoystickHatState_Centered;
	}

	Timestep WindowsInput::GetElapsedTime()
	{
		static float m_LastTime = 0.0f;
		float time = static_cast<float>(glfwGetTime());
		Timestep timestep = time - m_LastTime;
		m_LastTime = time;
		return timestep;
	}

	void WindowsInput::PollEvents()
	{
		glfwPollEvents();

		// TODO: I'm fairly sure that in glfw 3.4, which is 50% done as of writing this,
		// this will be unnecessary because joystick input (buttons, axes, and hats, in
		// addition to (dis)connect) will be done through callbacks instead of polling.
		for (sint32 jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
		{
			if (JoystickState& joystickState = m_Joysticks[ConvertJoystickID(jid)]; joystickState.IsConnected())
			{
				sint32 buttonCount;
				const uint8* pButtons;
				sint32 axisCount;
				const float* pAxes;
				sint32 hatCount;
				const uint8* pHats = glfwGetJoystickHats(jid, &hatCount);

				if (glfwJoystickIsGamepad(jid) == GLFW_TRUE)
				{
					GLFWgamepadstate gamepadState;

					bool success = glfwGetGamepadState(jid, &gamepadState) == GLFW_TRUE;
					CORE_ASSERT(success, "Joystick gamepad connect mismatch!");

					buttonCount = sizeof(gamepadState.buttons) / sizeof(*gamepadState.buttons);
					pButtons = gamepadState.buttons;
					axisCount = sizeof(gamepadState.axes) / sizeof(*gamepadState.axes);
					pAxes = gamepadState.axes;
				}
				else
				{
					pButtons = glfwGetJoystickButtons(jid, &buttonCount);
					pAxes = glfwGetJoystickAxes(jid, &axisCount);
				}

				for (sint32 i = 0; i < buttonCount; i++)
					SetJoystickButton(joystickState, ConvertJoystickButton(i), pButtons[i]);
				for (sint32 i = 0; i < axisCount; i++)
					SetJoystickAxis(joystickState, ConvertJoystickAxis(i), pAxes[i]);
				for (sint32 i = 0; i < hatCount; i++)
					SetJoystickHat(joystickState, ConvertJoystickHat(i), pHats[i]);
			}
		}
	}

	void WindowsInput::OnEvent(Event& rEvent)
	{
		rEvent.Dispatch(this, &WindowsInput::OnKeyPressEvent);
		rEvent.Dispatch(this, &WindowsInput::OnKeyReleaseEvent);
		rEvent.Dispatch(this, &WindowsInput::OnMouseButtonPressEvent);
		rEvent.Dispatch(this, &WindowsInput::OnMouseButtonReleaseEvent);
		m_fEventCallback(rEvent);
	}

	void WindowsInput::OnKeyPressEvent(KeyPressEvent& rEvent)
	{
		Keycode keycode = rEvent.GetKeycode();
		CORE_ASSERT(keycode < Keycode_Count, "Keycode index={0} out of bounds!", keycode);
		m_Keys[keycode / 8] |= (1 << (keycode % 8));
	}

	void WindowsInput::OnKeyReleaseEvent(KeyReleaseEvent& rEvent)
	{
		Keycode keycode = rEvent.GetKeycode();
		CORE_ASSERT(keycode < Keycode_Count, "Keycode index={0} out of bounds!", keycode);
		m_Keys[keycode / 8] &= ~(1 << (keycode % 8));
	}

	void WindowsInput::OnMouseButtonPressEvent(MouseButtonPressEvent& rEvent)
	{
		MouseButton button = rEvent.GetButton();
		CORE_ASSERT(button < Keycode_Count, "Keycode index={0} out of bounds!", button);
		m_MouseButtons[button / 8] |= (1 << (button % 8));
	}

	void WindowsInput::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& rEvent)
	{
		MouseButton button = rEvent.GetButton();
		CORE_ASSERT(button < Keycode_Count, "Keycode index={0} out of bounds!", button);
		m_MouseButtons[button / 8] &= ~(1 << (button % 8));
	}

	void WindowsInput::OnJoystickConnected(Joystick joystick)
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick={0} index out of bounds!", joystick);
		CORE_ASSERT(!m_Joysticks[joystick].IsConnected(), "Attempted to reconnect joystick!");

		sint32 jid = UnconvertJoystickID(joystick);

		sint32 buttonCount, axisCount, hatCount;
		UNUSED(glfwGetJoystickHats(jid, &hatCount));

		if (glfwJoystickIsGamepad(jid))
		{
			GLFWgamepadstate gamepadState;

			bool success = glfwGetGamepadState(jid, &gamepadState);
			CORE_ASSERT(success, "Joystick gamepad connect mismatch!");

			buttonCount = sizeof(gamepadState.buttons) / sizeof(*gamepadState.buttons);
			axisCount = sizeof(gamepadState.axes) / sizeof(*gamepadState.axes);
		}
		else
		{
			UNUSED(glfwGetJoystickButtons(jid, &buttonCount));
			UNUSED(glfwGetJoystickAxes(jid, &axisCount));
		}

		ConnectJoystick(m_Joysticks[joystick], buttonCount, axisCount, hatCount);
		JoystickConnectEvent event(joystick, true);
		OnEvent(event);
	}

	void WindowsInput::OnJoystickDisconnected(Joystick joystick)
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index={0} out of bounds!", joystick);
		CORE_ASSERT(m_Joysticks[joystick].IsConnected(), "Attempted to redisconnect joystick!");

		DisconnectJoystick(m_Joysticks[joystick]);
		JoystickConnectEvent event(joystick, false);
		OnEvent(event);
	}
}
