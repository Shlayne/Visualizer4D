#include "Engine/pch.h"
#include "Platform/System/Windows/WindowsWindow.h"

namespace eng
{
	static uint32 s_OpenWindowCount = 0;

	void Window::PollEvents() { glfwPollEvents(); }

	static bool s_GLFWInitialized = false;
	static GLFWerrorfun s_fPrevErrorCallback = nullptr;

	static void ErrorCallback(int errorCode, const char* pDescription)
	{
		LOG_CORE_ERROR("GLFW Error {0}: {1}", errorCode, pDescription);
		if (s_fPrevErrorCallback != nullptr)
			s_fPrevErrorCallback(errorCode, pDescription);
	}

	WindowsWindow::WindowsWindow(const WindowSpecifications& crSpecs, const Ref<Window>& crShareContextWindow)
	{
		if (!s_GLFWInitialized)
		{
			s_fPrevErrorCallback = glfwSetErrorCallback(&ErrorCallback);

			int status = glfwInit();
			CORE_ASSERT(status == GLFW_TRUE, "Failed to initialize GLFW!");
			s_GLFWInitialized = true;
		}

		GLFWwindow* pNativeShareContextWindow = crShareContextWindow ? ((WindowsWindow*)crShareContextWindow.get())->m_pWindow : NULL;
		m_pWindow = glfwCreateWindow(crSpecs.width, crSpecs.height, crSpecs.title.data(), NULL, pNativeShareContextWindow);
		CORE_ASSERT(m_pWindow != NULL, "Failed to create window!");
		s_OpenWindowCount++;

		m_rContext = Context::CreateRef(m_pWindow);

		glfwSwapInterval(1); // enable vsync

		glfwShowWindow(m_pWindow);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_pWindow);
		if (--s_OpenWindowCount == 0)
		{
			GLFWerrorfun fCallback = glfwSetErrorCallback(s_fPrevErrorCallback);
			CORE_ASSERT(fCallback == &ErrorCallback, "The GLFW error callback was set to {0}, without being reset to {1}, between glfwInit and glfwTerminate!", static_cast<void*>(fCallback), static_cast<void*>(&ErrorCallback));
			s_fPrevErrorCallback = nullptr;
			glfwTerminate();
		}
	}
}
