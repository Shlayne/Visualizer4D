#pragma once

#include "Engine/Core/Joysticks.h"
#include "Engine/Core/Keycodes.h"
#include "Engine/Core/MouseButtons.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Window.h"
#include "Engine/Events/Event.h"

namespace eng
{
	class Input
	{
	public:
		// Returns the Input singleton.
		static Input& Get();
	public:
		// Returns true if the key is currently pressed, false otherwise.
		virtual bool IsKeyPressed(Keycode keycode) const = 0;
		// Returns true if the mouse button is currently pressed, false otherwise.
		virtual bool IsMouseButtonPressed(MouseButton button) const = 0;

		// Gets the position of the mouse relative to the top left corner of the primary monitor.
		virtual glm::vec2 GetAbsoluteMousePosition() const = 0;
		// Gets the position of the mouse relative to the top left of the given window's content area.
		virtual glm::vec2 GetRelativeMousePosition(const void* cpNativeWindow) const = 0;
		// Gets the position of the mouse relative to the top left of the given window's content area.
		virtual glm::vec2 GetRelativeMousePosition(const Window& crWindow) const = 0;

		// Returns true if the joystick is connected, false otherwise.
		virtual bool IsJoystickConnected(Joystick joystick) const = 0;
		// Returns true if the joystick button is currently pressed, false otherwise.
		virtual bool IsJoystickButtonPressed(Joystick joystick, JoystickButton button) const = 0;
		// Returns the current value of a joystick axis.
		virtual float GetJoystickAxis(Joystick joystick, JoystickAxis axis) const = 0;
		// Returns the current value of a joystick hat.
		virtual JoystickHatState GetJoystickHat(Joystick joystick, JoystickHat hat) const = 0;
	protected:
		virtual Timestep GetElapsedTime() = 0;
		virtual void PollEvents() = 0;
		virtual void OnEvent(Event& rEvent) = 0;
	protected:
		inline void DisconnectJoystick(JoystickState& joystickState) { joystickState.Disconnect(); }
		inline void ConnectJoystick(JoystickState& joystickState, uint32 buttonCount, uint32 axisCount, uint32 hatCount) { joystickState.Connect(buttonCount, axisCount, hatCount); }
		inline void SetJoystickButton(JoystickState& joystickState, JoystickButton button, bool value) { joystickState.SetButton(button, value); }
		inline void SetJoystickAxis(JoystickState& joystickState, JoystickAxis axis, float value) { joystickState.SetAxis(axis, value); }
		inline void SetJoystickHat(JoystickState& joystickState, JoystickHat hat, JoystickHatState value) { joystickState.SetHat(hat, value); }
	private:
		static Scope<Input> CreateScope(EventCallback&& rrfEventCallback);
	private:
		friend class Application;
		friend class Window;
	};
}
