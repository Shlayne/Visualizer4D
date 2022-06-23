#pragma once

#if VULKAN_RENDERER_API_EXPORT
	#define VULKAN_RENDERER_API __declspec(dllexport)
#else
	#define VULKAN_RENDERER_API __declspec(dllimport)
#endif

#include "Engine/Core/Core.h"
#if SYSTEM_WINDOWS
	#define GLFW_INCLUDE_NONE
	#include <glfw/glfw3.h>
#endif

namespace eng
{
	class VULKAN_RENDERER_API VulkanRendererAPIBind
	{
	public: // Function type definitions.
#if ENABLE_LOGGING
		using FGetCoreLogger = Ref<spdlog::logger>&(*)();
#endif
#if SYSTEM_WINDOWS
		using FglfwMakeContextCurrent = void(*)(GLFWwindow*);
		using FglfwGetProcAddress = GLFWglproc(*)(const char*);
		using FglfwSwapBuffers = void(*)(GLFWwindow*);
#endif
	public: // Internal function calls.
#if ENABLE_LOGGING
		static Ref<spdlog::logger>& GetCoreLogger();
#endif
#if SYSTEM_WINDOWS
		static void glfwMakeContextCurrent(GLFWwindow* pWindow);
		static GLFWglproc glfwGetProcAddress(const char* pProcName);
		static void glfwSwapBuffers(GLFWwindow* pWindow);
#endif
	private: // Function setup.
		friend class RendererAPI;
		static void SetFunctions
		(
#if ENABLE_LOGGING
			FGetCoreLogger fGetCoreLogger,
#endif
#if SYSTEM_WINDOWS
			FglfwMakeContextCurrent fglfwMakeContextCurrent,
			FglfwGetProcAddress fglfwGetProcAddress,
			FglfwSwapBuffers fglfwSwapBuffers
#endif
		);
	private: // Function pointers themselves.
#if ENABLE_LOGGING
		static FGetCoreLogger s_fGetCoreLogger;
#endif
#if SYSTEM_WINDOWS
		static FglfwMakeContextCurrent s_fglfwMakeContextCurrent;
		static FglfwGetProcAddress s_fglfwGetProcAddress;
		static FglfwSwapBuffers s_fglfwSwapBuffers;
#endif
	private: // This is a fully static class, thank you very much.
		VulkanRendererAPIBind() = delete;
		VulkanRendererAPIBind(const VulkanRendererAPIBind&) = delete;
		VulkanRendererAPIBind(VulkanRendererAPIBind&&) = delete;
		VulkanRendererAPIBind& operator=(const VulkanRendererAPIBind&) = delete;
		VulkanRendererAPIBind& operator=(VulkanRendererAPIBind&&) = delete;
		~VulkanRendererAPIBind() = delete;
	};

	// Just to reduce function call code.
	using Bind = VulkanRendererAPIBind;
}
