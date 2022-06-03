#include "Engine/pch.h"
#include "Engine/Rendering/Context.h"
#include "Engine/Rendering/RendererAPI.h"
#if SYSTEM_SUPPORTS_OPENGL
	#include "Platform/Renderer/OpenGL/OpenGLContext.h"
#endif
#if SYSTEM_SUPPORTS_VULKAN
	//#include "Platform/Renderer/OpenGL/VulkanContext.h"
#endif

namespace eng
{
	Ref<Context> Context::CreateRef(void* pNativeWindow)
	{
		switch (RendererAPI::GetAPI())
		{
#if SYSTEM_SUPPORTS_OPENGL
		case RendererAPI::API::OpenGL: return eng::CreateRef<OpenGLContext>(pNativeWindow);
#endif
#if SYSTEM_SUPPORTS_VULKAN
		//case RendererAPI::API::OpenGL: return eng::CreateRef<VulkanContext>(pNativeWindow);
#endif
		UNKNOWN_RENDERER_API(RendererAPI::GetAPI(), nullptr);
		}
	}
}
