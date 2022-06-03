#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Rendering/Context.h"
#include <glfw/glfw3.h>

namespace eng
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecifications& crSpecs, const Ref<Window>& crShareContextWindow);
		virtual ~WindowsWindow();
	public:
		inline virtual const Ref<Context>& GetContext() const override { return m_rContext; }
	private:
		GLFWwindow* m_pWindow;
		Ref<Context> m_rContext;
	};
}
