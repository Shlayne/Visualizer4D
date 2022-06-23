#include "VulkanRendererAPI/VulkanRendererAPIBind.h"
#include "VulkanRendererAPI/VulkanLogger.h"

namespace eng
{
#if ENABLE_LOGGING
	VulkanRendererAPIBind::FGetCoreLogger VulkanRendererAPIBind::s_fGetCoreLogger = nullptr;
#endif
#if SYSTEM_WINDOWS
	VulkanRendererAPIBind::FglfwMakeContextCurrent VulkanRendererAPIBind::s_fglfwMakeContextCurrent = nullptr;
	VulkanRendererAPIBind::FglfwGetProcAddress VulkanRendererAPIBind::s_fglfwGetProcAddress = nullptr;
	VulkanRendererAPIBind::FglfwSwapBuffers VulkanRendererAPIBind::s_fglfwSwapBuffers = nullptr;
#endif

	void VulkanRendererAPIBind::SetFunctions
	(
#if ENABLE_LOGGING
		FGetCoreLogger fGetCoreLogger,
#endif
#if SYSTEM_WINDOWS
		FglfwMakeContextCurrent fglfwMakeContextCurrent,
		FglfwGetProcAddress fglfwGetProcAddress,
		FglfwSwapBuffers fglfwSwapBuffers
#endif
	)
	{
#if ENABLE_LOGGING
		s_fGetCoreLogger = fGetCoreLogger;
#endif
#if SYSTEM_WINDOWS
		s_fglfwMakeContextCurrent = fglfwMakeContextCurrent;
		s_fglfwGetProcAddress = fglfwGetProcAddress;
		s_fglfwSwapBuffers = fglfwSwapBuffers;
#endif
	}

#if ENABLE_LOGGING
	Ref<spdlog::logger>& VulkanRendererAPIBind::GetCoreLogger()
	{
		CORE_ASSERT(s_fGetCoreLogger != nullptr, "Vulkan Renderer API GetCoreLogger function was nullptr!");
		return s_fGetCoreLogger();
	}
#endif
#if SYSTEM_WINDOWS
	void VulkanRendererAPIBind::glfwMakeContextCurrent(GLFWwindow* pWindow)
	{
		CORE_ASSERT(s_fglfwMakeContextCurrent != nullptr, "Vulkan Renderer API glfwMakeContextCurrent function was nullptr!");
		s_fglfwMakeContextCurrent(pWindow);
	}

	GLFWglproc VulkanRendererAPIBind::glfwGetProcAddress(const char* pProcName)
	{
		CORE_ASSERT(s_fglfwGetProcAddress != nullptr, "Vulkan Renderer API glfwGetProcAddress function was nullptr!");
		return s_fglfwGetProcAddress(pProcName);
	}

	void VulkanRendererAPIBind::glfwSwapBuffers(GLFWwindow* pWindow)
	{
		CORE_ASSERT(s_fglfwSwapBuffers != nullptr, "Vulkan Renderer API glfwSwapBuffers function was nullptr!");
		s_fglfwSwapBuffers(pWindow);
	}
#endif
}
