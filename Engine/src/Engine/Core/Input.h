#pragma once

#include "Engine/Core/Joysticks.h"
#include "Engine/Core/Keycodes.h"
#include "Engine/Core/Modifiers.h"
#include "Engine/Core/MouseButtons.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Events/KeyEvents.h"
#include "Engine/Events/MouseEvents.h"

namespace eng
{
	class Input
	{
	public:
		// Returns true if the key is currently pressed, false otherwise.
		static bool IsKeyPressed(Keycode keycode);
		// Returns true if the mouse button is currently pressed, false otherwise.
		static bool IsMouseButtonPressed(MouseButton button);

		// Gets the position of the mouse relative to the top left corner of the primary monitor.
		static glm::vec2 GetAbsoluteMousePosition();
		// Gets the position of the mouse relative to the top left of the given window's content area.
		static glm::vec2 GetRelativeMousePosition(void* pNativeWindow);

		// Returns true if the joystick is connected, false otherwise.
		static bool IsJoystickConnected(Joystick joystick);
		// Returns true if the joystick button is currently pressed, false otherwise.
		static bool IsJoystickButtonPressed(Joystick joystick, JoystickButton button);
		// Returns the current value of a joystick axis.
		static float GetJoystickAxis(Joystick joystick, JoystickAxis axis);
		// Returns the current value of a joystick hat.
		static JoystickHatState GetJoystickHat(Joystick joystick, JoystickHat hat);
	private:
		friend class Application;

		static Timestep GetElapsedTime();
		static void Init(const eng::EventCallback& crfEventCallback);
		static void Shutdown();
		static void PollEvents();

		static void OnKeyPressEvent(KeyPressEvent& rEvent);
		static void OnKeyReleaseEvent(KeyReleaseEvent& rEvent);
		static void OnMouseButtonPressEvent(MouseButtonPressEvent& rEvent);
		static void OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& rEvent);
	private:
		static void OnJoystickConnected(Joystick joystick);
		static void OnJoystickDisconnected(Joystick joystick);
	private:
		friend class Window;
		static void EventCallback(Event& rEvent);
	private:
		Input() = delete;
		Input(const Input&) = delete;
		Input(Input&&) = delete;
		Input& operator=(const Input&) = delete;
		Input& operator=(Input&&) = delete;
		~Input() = delete;
	};
}
