#include "Engine/pch.h"
#include "Engine/Core/Input.h"
#include "Engine/Events/DeviceEvents.h"
#include "Platform/System/Windows/WindowsConversions.h"
#include <glfw/glfw3.h>
#include "Engine/Core/Application.h"

namespace eng
{
	static std::array<uint8, 1 + (Keycode_Count - 1) / 8> s_Keys;
	static std::array<uint8, 1 + (MouseButton_Count - 1) / 8> s_MouseButtons;
	static std::array<JoystickState, Joystick_Count> s_Joysticks;
	static EventCallback s_fEventCallback = nullptr;
	static bool s_Initialized = false;

	bool Input::IsKeyPressed(Keycode keycode)
	{
		CORE_ASSERT(keycode < Keycode_Count, "Keycode index out of bounds!");
		return !!(s_Keys[keycode / 8] & (1 << (keycode % 8)));
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		CORE_ASSERT(button < MouseButton_Count, "Mouse Button index out of bounds!");
		return !!(s_MouseButtons[button / 8] & (1 << (button % 8)));
	}

	glm::vec2 Input::GetAbsoluteMousePosition()
	{
		GLFWwindow* pWindow = glfwGetCurrentContext();
		CORE_ASSERT(pWindow != NULL, "Window was null!");
		glm::s32vec2 windowPos{ 0 };
		glfwGetWindowPos(pWindow, &windowPos.x, &windowPos.y);
		glm::dvec2 cursorPos{ 0.0 };
		glfwGetCursorPos(pWindow, &cursorPos.x, &cursorPos.y);
		return glm::vec2(glm::dvec2(windowPos) + cursorPos);
	}

	glm::vec2 Input::GetRelativeMousePosition(void* pNativeWindow)
	{
		GLFWwindow* pWindow = static_cast<GLFWwindow*>(pNativeWindow);
		CORE_ASSERT(pWindow != NULL, "Window was null!");
		glm::dvec2 cursorPos{ 0.0 };
		glfwGetCursorPos(pWindow, &cursorPos.x, &cursorPos.y);
		return glm::vec2(cursorPos);
	}

	bool Input::IsJoystickConnected(Joystick joystick)
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index out of bounds!");
		return s_Joysticks[joystick].IsConnected();
	}

	bool Input::IsJoystickButtonPressed(Joystick joystick, JoystickButton button)
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index out of bounds!");
		const JoystickState& joystickState = s_Joysticks[joystick];
		return joystickState.IsConnected() && joystickState.GetButton(button);
	}

	float Input::GetJoystickAxis(Joystick joystick, JoystickAxis axis)
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index out of bounds!");
		const JoystickState& joystickState = s_Joysticks[joystick];
		return joystickState.IsConnected() ? joystickState.GetAxis(axis) : 0.0f;
	}

	JoystickHatState Input::GetJoystickHat(Joystick joystick, JoystickHat hat)
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index out of bounds!");
		const JoystickState& joystickState = s_Joysticks[joystick];
		return joystickState.IsConnected() ? joystickState.GetHat(hat) : JoystickHatState_Centered;
	}

	Timestep Input::GetElapsedTime()
	{
		static float s_LastTime = 0.0f;
		float time = static_cast<float>(glfwGetTime());
		Timestep timestep = time - s_LastTime;
		s_LastTime = time;
		return timestep;
	}

	void Input::Init(const eng::EventCallback& crfEventCallback)
	{
		s_fEventCallback = crfEventCallback;

		if (!s_Initialized)
		{
			PROFILE_SCOPE("glfwInit");

#if ENABLE_ASSERTS
			UNUSED(glfwSetErrorCallback([](int errorCode, const char* pDescription)
			{
				CORE_ASSERT(false, "GLFW Error ({0}): {1}", errorCode, pDescription);
			}));
#endif

			glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
			int status = glfwInit();
			CORE_ASSERT(status == GLFW_TRUE, "Failed to initialize GLFW!");
			s_Initialized = true;
		}

		UNUSED(glfwSetJoystickCallback([](sint32 jid, sint32 event)
		{
			switch (event)
			{
				case GLFW_CONNECTED: OnJoystickConnected(ConvertJoystickID(jid)); break;
				case GLFW_DISCONNECTED: OnJoystickDisconnected(ConvertJoystickID(jid)); break;
			}
		}));

		// Now that the joystick callback is set, emit some fake connect events
		// for joysticks that were connected prior to running the application.
		for (sint32 jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
			if (glfwJoystickPresent(jid) == GLFW_TRUE)
				OnJoystickConnected(ConvertJoystickID(jid));
	}

	void Input::Shutdown()
	{
		CORE_ASSERT(s_Initialized, "Attempted to shutdown input before it was initialized!");
		UNUSED(glfwSetErrorCallback(NULL));
		glfwTerminate();

		// Reset all static data.

		s_Keys.fill(0);
		s_MouseButtons.fill(0);
		for (Joystick joystick = Joystick_1; joystick < Joystick_Count; joystick++)
			OnJoystickDisconnected(joystick);

		s_fEventCallback = nullptr;
		s_Initialized = false;
	}

	void Input::OnJoystickConnected(Joystick joystick)
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index out of bounds!");
		if (s_Joysticks[joystick].IsConnected())
			return;

		sint32 jid = UnconvertJoystickID(joystick);

		sint32 buttonCount;
		sint32 axisCount;
		sint32 hatCount;
		UNUSED(glfwGetJoystickHats(jid, &hatCount));

		if (glfwJoystickIsGamepad(jid) == GLFW_TRUE)
		{
			GLFWgamepadstate gamepadState;

			bool success = glfwGetGamepadState(jid, &gamepadState) == GLFW_TRUE;
			CORE_ASSERT(success, "Joystick gamepad connect mismatch!");

			buttonCount = sizeof(gamepadState.buttons) / sizeof(*gamepadState.buttons);
			axisCount = sizeof(gamepadState.axes) / sizeof(*gamepadState.axes);
		}
		else
		{
			UNUSED(glfwGetJoystickButtons(jid, &buttonCount));
			UNUSED(glfwGetJoystickAxes(jid, &axisCount));
		}

		s_Joysticks[joystick].Connect(buttonCount, axisCount, hatCount);
		JoystickConnectEvent event(joystick, true);
		EventCallback(event);
	}

	void Input::OnJoystickDisconnected(Joystick joystick)
	{
		CORE_ASSERT(joystick < Joystick_Count, "Joystick index out of bounds!");
		if (!s_Joysticks[joystick].IsConnected())
			return;

		s_Joysticks[joystick].Disconnect();
		JoystickConnectEvent event(joystick, false);
		EventCallback(event);
	}

	void Input::PollEvents()
	{
		glfwPollEvents();

		// TODO: I'm fairly sure that in glfw 3.4, which is 50% done as of writing this,
		// this will be unnecessary because joystick input (buttons, axes, and hats, in
		// addition to (dis)connect) will be done through callbacks instead of polling.
		for (sint32 jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
		{
			if (JoystickState& joystickState = s_Joysticks[ConvertJoystickID(jid)]; joystickState.IsConnected())
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
					joystickState.SetButton(ConvertJoystickButton(i), pButtons[i]);
				for (sint32 i = 0; i < axisCount; i++)
					joystickState.SetAxis(ConvertJoystickAxis(i), pAxes[i]);
				for (sint32 i = 0; i < hatCount; i++)
					joystickState.SetHat(ConvertJoystickHat(i), pHats[i]);
			}
		}
	}

	void Input::EventCallback(Event& rEvent)
	{
		s_fEventCallback(rEvent);
	}

	void Input::OnKeyPressEvent(KeyPressEvent& rEvent)
	{
		Keycode keycode = rEvent.GetKeycode();
		CORE_ASSERT(keycode < Keycode_Count, "Keycode index out of bounds!");
		s_Keys[keycode / 8] |= (1 << (keycode % 8));
	}

	void Input::OnKeyReleaseEvent(KeyReleaseEvent& rEvent)
	{
		Keycode keycode = rEvent.GetKeycode();
		CORE_ASSERT(keycode < Keycode_Count, "Keycode index out of bounds!");
		s_Keys[keycode / 8] &= ~(1 << (keycode % 8));
	}

	void Input::OnMouseButtonPressEvent(MouseButtonPressEvent& rEvent)
	{
		MouseButton button = rEvent.GetButton();
		CORE_ASSERT(button < Keycode_Count, "Keycode index out of bounds!");
		s_MouseButtons[button / 8] |= (1 << (button % 8));
	}

	void Input::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& rEvent)
	{
		MouseButton button = rEvent.GetButton();
		CORE_ASSERT(button < Keycode_Count, "Keycode index out of bounds!");
		s_MouseButtons[button / 8] &= ~(1 << (button % 8));
	}
}
