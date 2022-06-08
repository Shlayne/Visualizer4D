#include "OpenGLRendererAPI/OpenGLContext.h"
#include "OpenGLRendererAPI/OpenGLLogger.h"

namespace eng
{
	OpenGLContext::OpenGLContext(void* pNativeWindow)
#if SYSTEM_WINDOWS
		: m_pWindow(static_cast<GLFWwindow*>(pNativeWindow))
	{
		CORE_ASSERT(m_pWindow != NULL, "Context's Window is null!");
		Bind::glfwMakeContextCurrent(m_pWindow);
		int status = gladLoadGLLoader((GLADloadproc)Bind::glfwGetProcAddress);
		CORE_ASSERT(status != 0, "Failed to initialize GLAD loader!");
	}
#endif

	void OpenGLContext::SwapBuffers()
	{
#if SYSTEM_WINDOWS
		Bind::glfwSwapBuffers(m_pWindow);
#endif
	}

	void OpenGLContext::MakeCurrent()
	{
#if SYSTEM_WINDOWS
		Bind::glfwMakeContextCurrent(m_pWindow);
#endif
	}
}
