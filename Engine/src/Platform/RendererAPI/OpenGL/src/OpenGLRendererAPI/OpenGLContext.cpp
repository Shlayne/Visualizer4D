#include "OpenGLRendererAPI/OpenGLContext.h"
#include "OpenGLRendererAPI/OpenGLLogger.h"

namespace eng
{
	OpenGLContext::OpenGLContext(void* pNativeWindow)
#if SYSTEM_WINDOWS
		: m_pWindow((GLFWwindow*)pNativeWindow)
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

	const char* OpenGLContext::GetVendor() const
	{
		return (const char*)glGetString(GL_VENDOR);
	}

	const char* OpenGLContext::GetRenderer() const
	{
		return (const char*)glGetString(GL_RENDERER);
	}

	const char* OpenGLContext::GetVersion() const
	{
		return (const char*)glGetString(GL_VERSION);
	}
}
