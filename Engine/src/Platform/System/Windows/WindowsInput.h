#pragma once

#include "Engine/Core/Input.h"
#include "Engine/Events/KeyEvents.h"
#include "Engine/Events/MouseEvents.h"
#include <array>

namespace eng
{
	class WindowsInput : public Input
	{
	public:
		WindowsInput(eng::EventCallback&& rrfEventCallback);
		virtual ~WindowsInput();
	public:
		virtual bool IsKeyPressed(Keycode keycode) const override;
		virtual bool IsMouseButtonPressed(MouseButton button) const override;

		virtual glm::vec2 GetAbsoluteMousePosition() const override;
		virtual glm::vec2 GetRelativeMousePosition(const void* cpNativeWindow) const override;
		virtual glm::vec2 GetRelativeMousePosition(const Window& crWindow) const override;

		virtual bool IsJoystickConnected(Joystick joystick) const override;
		virtual bool IsJoystickButtonPressed(Joystick joystick, JoystickButton button) const override;
		virtual float GetJoystickAxis(Joystick joystick, JoystickAxis axis) const override;
		virtual JoystickHatState GetJoystickHat(Joystick joystick, JoystickHat hat) const override;
	protected:
		virtual Timestep GetElapsedTime() override;
		virtual void PollEvents() override;
		virtual void OnEvent(Event& rEvent) override;
	private:
		void OnKeyPressEvent(KeyPressEvent& rEvent);
		void OnKeyReleaseEvent(KeyReleaseEvent& rEvent);
		void OnMouseButtonPressEvent(MouseButtonPressEvent& rEvent);
		void OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& rEvent);
	private:
		void OnJoystickConnected(Joystick joystick);
		void OnJoystickDisconnected(Joystick joystick);
	private:
		std::array<uint8, 1 + (Keycode_Count - 1) / 8> m_Keys{};
		std::array<uint8, 1 + (MouseButton_Count - 1) / 8> m_MouseButtons{};
		std::array<JoystickState, Joystick_Count> m_Joysticks;
		EventCallback m_fEventCallback = nullptr;
	};
}
