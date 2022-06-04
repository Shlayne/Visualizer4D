#include "OpenGLRendererAPI/OpenGLRendererAPIBind.h"
#include "OpenGLRendererAPI/OpenGLLogger.h"

namespace eng
{
#if ENABLE_LOGGING
	OpenGLRendererAPIBind::GetCoreLogger_FUNC OpenGLRendererAPIBind::s_GetCoreLogger = nullptr;

	void OpenGLRendererAPIBind::SetLoggerFunctions
	(
		GetCoreLogger_FUNC GetCoreLogger_FPTR
	)
	{
		s_GetCoreLogger = GetCoreLogger_FPTR;
	}

	Ref<spdlog::logger>& OpenGLRendererAPIBind::GetCoreLogger()
	{
		CORE_ASSERT(s_GetCoreLogger != nullptr, "OpenGL Renderer API GetCoreLogger function was nullptr!");
		return s_GetCoreLogger();
	}
#endif
#if SYSTEM_WINDOWS
	OpenGLRendererAPIBind::glfwMakeContextCurrent_FUNC OpenGLRendererAPIBind::s_glfwMakeContextCurrent = nullptr;
	OpenGLRendererAPIBind::glfwGetProcAddress_FUNC OpenGLRendererAPIBind::s_glfwGetProcAddress = nullptr;
	OpenGLRendererAPIBind::glfwSwapBuffers_FUNC OpenGLRendererAPIBind::s_glfwSwapBuffers = nullptr;

	void OpenGLRendererAPIBind::SetGLFWFunctions
	(
		glfwMakeContextCurrent_FUNC glfwMakeContextCurrent_FPTR,
		glfwGetProcAddress_FUNC glfwGetProcAddress_FPTR,
		glfwSwapBuffers_FUNC glfwSwapBuffers_FPTR
	)
	{
		s_glfwMakeContextCurrent = glfwMakeContextCurrent_FPTR;
		s_glfwGetProcAddress = glfwGetProcAddress_FPTR;
		s_glfwSwapBuffers = glfwSwapBuffers_FPTR;
	}

	void OpenGLRendererAPIBind::glfwMakeContextCurrent(GLFWwindow* pWindow)
	{
		CORE_ASSERT(s_glfwMakeContextCurrent != nullptr, "OpenGL Renderer API glfwMakeContextCurrent function was nullptr!");
		s_glfwMakeContextCurrent(pWindow);
	}

	GLFWglproc OpenGLRendererAPIBind::glfwGetProcAddress(const char* pProcName)
	{
		CORE_ASSERT(s_glfwGetProcAddress != nullptr, "OpenGL Renderer API glfwGetProcAddress function was nullptr!");
		return s_glfwGetProcAddress(pProcName);
	}

	void OpenGLRendererAPIBind::glfwSwapBuffers(GLFWwindow* pWindow)
	{
		CORE_ASSERT(s_glfwSwapBuffers != nullptr, "OpenGL Renderer API glfwSwapBuffers function was nullptr!");
		s_glfwSwapBuffers(pWindow);
	}
#endif
}
