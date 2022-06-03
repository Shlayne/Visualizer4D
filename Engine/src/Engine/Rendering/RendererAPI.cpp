#include "Engine/pch.h"
#include "Engine/Rendering/RendererAPI.h"
#if SYSTEM_SUPPORTS_OPENGL
	#include "Platform/Renderer/OpenGL/OpenGLRendererAPI.h"
#endif
#if SYSTEM_SUPPORTS_VULKAN
	//#include "Platform/Renderer/Vulkan/VulkanRendererAPI.h"
#endif

namespace eng
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::None;

	bool RendererAPI::SupportsAPI(API api)
	{
		switch (api)
		{
#if SYSTEM_SUPPORTS_OPENGL
		case API::OpenGL: break;
#endif
#if SYSTEM_SUPPORTS_VULKAN
		//case API::Vulkan: break;
#endif
		default: return false;
		}

		return true;
	}

	bool RendererAPI::SetAPI(API api)
	{
		if (!SupportsAPI(api))
			return false;
		s_API = api;
		return true;
	}

	Scope<RendererAPI> RendererAPI::CreateScope()
	{
		switch (s_API)
		{
#if SYSTEM_SUPPORTS_OPENGL
		case API::OpenGL: return eng::CreateScope<OpenGLRendererAPI>();
#endif
#if SYSTEM_SUPPORTS_VULKAN
		//case API::Vulkan: return eng::CreateScope<VulkanRendererAPI>();
#endif
		UNKNOWN_RENDERER_API(s_API);
		}
	}
}
