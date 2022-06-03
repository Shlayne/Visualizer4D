#include "Engine/pch.h"
#include "Platform/Renderer/OpenGL/OpenGLContext.h"

namespace eng
{
	OpenGLContext::OpenGLContext(void* pNativeWindow)
#if SYSTEM_WINDOWS
		: m_pWindow((GLFWwindow*)pNativeWindow)
	{
		PROFILE_FUNCTION();

		CORE_ASSERT(m_pWindow != NULL, "Context's Window is null!");
		glfwMakeContextCurrent(m_pWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CORE_ASSERT(status != 0, "Failed to initialize GLAD!");
	}
#endif

	void OpenGLContext::SwapBuffers()
	{
		PROFILE_FUNCTION();

#if SYSTEM_WINDOWS
		glfwSwapBuffers(m_pWindow);
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
