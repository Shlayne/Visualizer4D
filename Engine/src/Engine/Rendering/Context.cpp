#include "Engine/pch.h"
#include "Engine/Rendering/Context.h"
#include "Engine/Rendering/RendererAPI.h"
#if SYSTEM_SUPPORTS_OPENGL
	#include "OpenGLRendererAPI/OpenGLContext.h"
#endif
#if SYSTEM_SUPPORTS_VULKAN
	#include "VulkanRendererAPI/VulkanContext.h"
#endif

namespace eng
{
	Scope<Context> Context::CreateScope(void* pNativeWindow)
	{
		switch (RendererAPI::GetAPI())
		{
#if SYSTEM_SUPPORTS_OPENGL
		case RendererAPI::API::OpenGL: return eng::CreateScope<OpenGLContext>(pNativeWindow);
#endif
#if SYSTEM_SUPPORTS_VULKAN
		case RendererAPI::API::Vulkan: return eng::CreateScope<VulkanContext>(pNativeWindow);
#endif
		UNKNOWN_RENDERER_API(RendererAPI::GetAPI(), nullptr);
		}
	}
}
