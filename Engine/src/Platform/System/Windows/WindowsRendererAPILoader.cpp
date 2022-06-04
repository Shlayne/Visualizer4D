#include "Engine/pch.h"
#include "Engine/Rendering/RendererAPI.h"
#if SYSTEM_SUPPORTS_OPENGL
	#include "OpenGLRendererAPI/OpenGLRendererAPIBind.h"
#endif
#if SYSTEM_SUPPORTS_VULKAN
	#include "VulkanRendererAPI/VulkanRendererAPIBind.h"
#endif
#include <libloaderapi.h>

namespace eng
{
	static HMODULE s_hLibModule = NULL;

	void RendererAPI::LoadAPI()
	{
		CORE_ASSERT(s_hLibModule == NULL, "Attempted to reload Renderer API without restarting!");

		switch (s_API)
		{
#if SYSTEM_SUPPORTS_OPENGL
		case API::OpenGL:
			s_hLibModule = LoadLibraryW(L"OpenGLRendererAPI.dll");
			CORE_ASSERT(s_hLibModule != NULL, "Failed to load OpenGL Renderer API dll!");

#if ENABLE_LOGGING
			OpenGLRendererAPIBind::SetLoggerFunctions
			(
				&Logger::GetCoreLogger
			);
#endif
//#if SYSTEM_WINDOWS // Not necessary, as this is already windows only.
			OpenGLRendererAPIBind::SetGLFWFunctions
			(
				&glfwMakeContextCurrent,
				&glfwGetProcAddress,
				&glfwSwapBuffers
			);
//#endif
			break;
#endif
#if SYSTEM_SUPPORTS_VULKAN
		case API::Vulkan:
			s_hLibModule = LoadLibraryW(L"VulkanRendererAPI.dll");
			CORE_ASSERT(s_hLibModule != NULL, "Failed to load Vulkan Renderer API dll!");

			// TODO: VulkanRendererAPIBind Functions.

			break;
#endif
		UNKNOWN_RENDERER_API(s_API);
		}
	}

	void RendererAPI::UnloadAPI()
	{
		CORE_ASSERT(s_hLibModule != NULL, "Attempted to unload Renderer API without having loaded it!");
		BOOL status = FreeLibrary(s_hLibModule);
		CORE_ASSERT(status != 0, "Failed to free Renderer API dll library!");
		s_hLibModule = NULL;
	}
}
