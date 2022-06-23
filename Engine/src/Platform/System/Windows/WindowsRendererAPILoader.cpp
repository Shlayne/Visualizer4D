#include "Engine/pch.h"
#include "Engine/Rendering/RendererAPI.h"
#if SUPPORTS_VULKAN
	#include "VulkanRendererAPI/VulkanRendererAPIBind.h"
#endif
#include <libloaderapi.h>

namespace eng
{
	static HMODULE s_hLibModule = NULL;

	void RendererAPI::LoadAPI()
	{
		CORE_ASSERT(s_hLibModule == NULL, "Attempted to load Renderer API ({0}) without restarting!", RendererAPI::GetAPI());

		switch (RendererAPI::GetAPI())
		{
#if SUPPORTS_VULKAN
		case API_Vulkan:
			s_hLibModule = LoadLibraryW(L"VulkanRendererAPI.dll");
			CORE_ASSERT(s_hLibModule != NULL, "Failed to load Vulkan Renderer API dll!");

			VulkanRendererAPIBind::SetFunctions(
#if ENABLE_LOGGING
				&Logger::GetCoreLogger,
#endif
#if SYSTEM_WINDOWS
				&glfwMakeContextCurrent,
				&glfwGetProcAddress,
				&glfwSwapBuffers
#endif
			);

			break;
#endif
		UNKNOWN_RENDERER_API(RendererAPI::GetAPI());
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
