#include "Engine/pch.h"
#include "Engine/Rendering/RendererAPI.h"
#if SUPPORTS_VULKAN
	//#include "VulkanRendererAPI/VulkanRendererAPI.h"
#endif

namespace eng
{
	static RendererAPI::API s_API = RendererAPI::API_None;

	RendererAPI::API RendererAPI::GetAPI()
	{
		return s_API;
	}

	bool RendererAPI::SetAPI(API api)
	{
		if (!SupportsAPI(api))
			return false;
		s_API = api;
		return true;
	}

	bool RendererAPI::SupportsAPI(API api)
	{
		switch (api)
		{
#if SUPPORTS_VULKAN
		case API_Vulkan: break;
#endif
		default: return false;
		}

		return true;
	}

	Scope<RendererAPI> RendererAPI::CreateScope()
	{
		switch (s_API)
		{
#if SUPPORTS_VULKAN
		case API_Vulkan: return nullptr; // TODO // eng::CreateScope<VulkanRendererAPI>();
#endif
		UNKNOWN_RENDERER_API(s_API, nullptr);
		}
	}
}
