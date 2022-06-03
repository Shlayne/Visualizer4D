#include "Engine/pch.h"
#include "Engine/Rendering/RendererAPI.h"
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
		case API::OpenGL: s_hLibModule = LoadLibraryW(L"RendererAPIOpenGL.dll"); break;
#endif
#if SYSTEM_SUPPORTS_VULKAN
		//case API::Vulkan: s_hLibModule = LoadLibraryW(L"RendererAPIVulkan.dll"); break;
#endif
		UNKNOWN_RENDERER_API(s_API);
		}

		CORE_ASSERT(s_hLibModule != NULL, "Failed to load Renderer API dll!");
	}

	void RendererAPI::UnloadAPI()
	{
		CORE_ASSERT(s_hLibModule != NULL, "Attempted to unload Renderer API without having loaded it!");
		BOOL status = FreeLibrary(s_hLibModule);
		CORE_ASSERT(status != 0, "Failed to free Renderer API dll library!");
		s_hLibModule = NULL;
	}
}
