#pragma once

#include "Engine/Rendering/RendererAPI.h"
#include "Engine/Rendering/RenderCommand.h"
#include <atomic>

namespace eng
{
	class Renderer
	{
	public:
		static void EnableDepthTest();
		static void DisableDepthTest();
		static void EnableBlending();
		static void DisableBlending();
		static void EnableCulling();
		static void DisableCulling();

		static void SetViewport(const glm::s32vec2& position, const glm::s32vec2& size);

		static void Clear();
		static void ClearDepth();
		static void SetClearColor(const glm::vec4& color);

		//static void DrawIndexed(); // TODO
	public: // Capabilities
		static sint32 GetMaxTextureSlots();
		static sint32 GetMaxTextureSize();
		static sint32 GetMaxFramebufferWidth();
		static sint32 GetMaxFramebufferHeight();
		static sint32 GetMaxFramebufferColorAttachments();
	private: // Implementation
		static void* Shutdown_Impl(const void* pData);

		static void* EnableDepthTest_Impl(const void* pData);
		static void* DisableDepthTest_Impl(const void* pData);
		static void* EnableBlending_Impl(const void* pData);
		static void* DisableBlending_Impl(const void* pData);
		static void* EnableCulling_Impl(const void* pData);
		static void* DisableCulling_Impl(const void* pData);

		static void* SetViewport_Impl(const void* pData);

		static void* Clear_Impl(const void* pData);
		static void* ClearDepth_Impl(const void* pData);
		static void* SetClearColor_Impl(const void* pData);

		static void* DrawIndexed_Impl(const void* pData);
	public: // Capabilities Implementation
		static void* GetMaxTextureSlots_Impl(const void* pData);
		static void* GetMaxTextureSize_Impl(const void* pData);
		static void* GetMaxFramebufferWidth_Impl(const void* pData);
		static void* GetMaxFramebufferHeight_Impl(const void* pData);
		static void* GetMaxFramebufferColorAttachments_Impl(const void* pData);
	private:
		static sint32 GetCachableSInt32(sint32& rCache, RenderCommandType type);
	private:
		friend class Application;
		static void Init();
		static void Shutdown();
	private:
		static void RenderThread();
	private:
		struct Cache
		{
			sint32 maxTextureSlots = 0;
			sint32 maxTextureSize = 0;
			sint32 maxFramebufferWidth = 0;
			sint32 maxFramebufferHeight = 0;
			sint32 maxFramebufferColorAttachments = 0;
		};
	private:
		static std::array<void*(*)(const void*), RenderCommand_Count> s_CommandFunctions;
		static Scope<RendererAPI> s_API;
		static std::atomic<void*> s_LastReturn;
		static Cache s_Cache;
	};
}
