#include "Engine/pch.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Core/Application.h"
#include "Engine/Rendering/RenderCommandQueue.h"
#include "Engine/Rendering/RenderCommandData.h"
#include <thread>

namespace eng
{
	std::array<void*(*)(const void*), RenderCommand_Count> Renderer::s_CommandFunctions
	{
		&Renderer::Shutdown_Impl,

		&Renderer::EnableDepthTest_Impl,
		&Renderer::DisableDepthTest_Impl,
		&Renderer::EnableBlending_Impl,
		&Renderer::DisableBlending_Impl,
		&Renderer::EnableCulling_Impl,
		&Renderer::DisableCulling_Impl,

		&Renderer::SetViewport_Impl,

		&Renderer::Clear_Impl,
		&Renderer::ClearDepth_Impl,
		&Renderer::SetClearColor_Impl,

		&Renderer::DrawIndexed_Impl,

		&Renderer::GetMaxTextureSlots_Impl,
		&Renderer::GetMaxTextureSize_Impl,
		&Renderer::GetMaxFramebufferWidth_Impl,
		&Renderer::GetMaxFramebufferHeight_Impl,
		&Renderer::GetMaxFramebufferColorAttachments_Impl,
	};
	Scope<RendererAPI> Renderer::s_API = nullptr;
	std::atomic<void*> Renderer::s_LastReturn = nullptr;
	Renderer::Cache Renderer::s_Cache;

	// Stuff that would be a static member in Renderer,
	// but I don't want to include their headers in Renderer.h.
	static RenderCommandQueue s_CommandQueue;
	static std::thread s_RenderThread;

	// For renderer api functions that have return values.
	static std::condition_variable s_ReturnCondition;
	static std::mutex s_ReturnMutex;
	static std::atomic_bool s_ReturnAcquired = false;

	void Renderer::Init()
	{
		//CORE_ASSERT(s_API == nullptr, "Attempted to recreate renderer!");

		//s_API = RendererAPI::CreateScope();
		//s_RenderThread = std::thread(&Renderer::RenderThread);
	}

	void Renderer::Shutdown()
	{
		//// May be called without Init being called.
		//if (s_API == nullptr) return;

		//s_CommandQueue.IssueCommand(RenderCommand_Shutdown, true);
		//s_RenderThread.join();
		//s_CommandQueue.ClearPendingCommands();
		//DestroyScope(s_API);

		//// Reset cached values.
		//memset(&s_Cache, 0, sizeof(s_Cache));
	}

	void Renderer::RenderThread()
	{
		//Application::Get().GetWindow().GetContext().MakeCurrent();

		//RenderCommand command;
		//do
		//{
		//	// Wait for a command to be issued.
		//	command = s_CommandQueue.WaitForCommand();
		//	// Execute the command.
		//	s_LastReturn = s_CommandFunctions[command.GetType()](command.GetData());
		//	// Delete the command's input data.
		//	delete command.GetData();

		//	// If the command had a return value...
		//	if (s_LastReturn)
		//	{
		//		// Let the caller know and wait until they receive it before it's deleted.
		//		{
		//			std::lock_guard<std::mutex> lock(s_ReturnMutex);
		//			s_ReturnCondition.notify_one();
		//		}
		//		while (!s_ReturnAcquired); // Should be very fast, if not instant.
		//		s_ReturnAcquired = false;
		//		delete static_cast<void*>(s_LastReturn);
		//	}
		//}
		//while (command.GetType() != RenderCommand_Shutdown);

		//RemoveCurrentContext();
	}

	void Renderer::EnableDepthTest() { s_CommandQueue.IssueCommand(RenderCommand_EnableDepthTest); }
	void Renderer::DisableDepthTest() { s_CommandQueue.IssueCommand(RenderCommand_DisableDepthTest); }
	void Renderer::EnableBlending() { s_CommandQueue.IssueCommand(RenderCommand_EnableBlending); }
	void Renderer::DisableBlending() { s_CommandQueue.IssueCommand(RenderCommand_DisableBlending); }
	void Renderer::EnableCulling() { s_CommandQueue.IssueCommand(RenderCommand_DisableCulling); }
	void Renderer::DisableCulling() { s_CommandQueue.IssueCommand(RenderCommand_EnableCulling); }
	
	void Renderer::SetViewport(const glm::s32vec2& position, const glm::s32vec2& size)
	{ s_CommandQueue.IssueCommand({ RenderCommand_SetViewport, new SetViewportData(position, size) }); }

	void Renderer::Clear() { s_CommandQueue.IssueCommand(RenderCommand_Clear); }
	void Renderer::ClearDepth() { s_CommandQueue.IssueCommand(RenderCommand_ClearDepth); }
	void Renderer::SetClearColor(const glm::vec4& color)
	{ s_CommandQueue.IssueCommand({ RenderCommand_SetClearColor, new SetClearColorData(color) }); }

	sint32 Renderer::GetMaxTextureSlots()
	{ return GetCachableSInt32(s_Cache.maxTextureSlots, RenderCommand_GetMaxTextureSlots); }
	sint32 Renderer::GetMaxTextureSize()
	{ return GetCachableSInt32(s_Cache.maxTextureSize, RenderCommand_GetMaxTextureSize); }
	sint32 Renderer::GetMaxFramebufferWidth()
	{ return GetCachableSInt32(s_Cache.maxFramebufferWidth, RenderCommand_GetMaxFramebufferWidth); }
	sint32 Renderer::GetMaxFramebufferHeight()
	{ return GetCachableSInt32(s_Cache.maxFramebufferHeight, RenderCommand_GetMaxFramebufferHeight); }
	sint32 Renderer::GetMaxFramebufferColorAttachments()
	{ return GetCachableSInt32(s_Cache.maxFramebufferColorAttachments, RenderCommand_GetMaxFramebufferColorAttachments); }

	sint32 Renderer::GetCachableSInt32(sint32& rCache, RenderCommandType type)
	{
		// Since all of this is a decent chunk of work, cache it.
		if (!rCache) // TODO: make a better way of checking if it's been cached already.
		{
			std::unique_lock<std::mutex> lock(s_ReturnMutex);
			s_CommandQueue.IssueCommand(type, true);
			// Wait for current render command to complete, if present, then wait until this command completes.
			s_ReturnCondition.wait(lock);

			rCache = *static_cast<sint32*>(static_cast<void*>(s_LastReturn));
			s_ReturnAcquired = true; // Notify the render thread that it can resume operation.
		}
		return rCache;
	}

	// Implementation

	void* Renderer::Shutdown_Impl(const void* pData)
	{
		UNUSED(pData);
		return nullptr;
	}

	void* Renderer::EnableDepthTest_Impl(const void* pData)
	{
		UNUSED(pData);
		s_API->EnableDepthTest();
		return nullptr;
	}

	void* Renderer::DisableDepthTest_Impl(const void* pData)
	{
		UNUSED(pData);
		s_API->DisableDepthTest();
		return nullptr;
	}

	void* Renderer::EnableBlending_Impl(const void* pData)
	{
		UNUSED(pData);
		s_API->EnableBlending();
		return nullptr;
	}

	void* Renderer::DisableBlending_Impl(const void* pData)
	{
		UNUSED(pData);
		s_API->DisableBlending();
		return nullptr;
	}

	void* Renderer::EnableCulling_Impl(const void* pData)
	{
		UNUSED(pData);
		s_API->EnableCulling();
		return nullptr;
	}

	void* Renderer::DisableCulling_Impl(const void* pData)
	{
		UNUSED(pData);
		s_API->DisableCulling();
		return nullptr;
	}


	void* Renderer::SetViewport_Impl(const void* pData)
	{
		auto& rData = *static_cast<const SetViewportData*>(pData);
		s_API->SetViewport(rData.position, rData.size);
		return nullptr;
	}


	void* Renderer::Clear_Impl(const void* pData)
	{
		UNUSED(pData);
		s_API->Clear();
		return nullptr;
	}

	void* Renderer::ClearDepth_Impl(const void* pData)
	{
		UNUSED(pData);
		s_API->ClearDepth();
		return nullptr;
	}

	void* Renderer::SetClearColor_Impl(const void* pData)
	{
		auto& rData = *static_cast<const SetClearColorData*>(pData);
		s_API->SetClearColor(rData.color);
		return nullptr;
	}


	void* Renderer::DrawIndexed_Impl(const void* pData)
	{
		// TODO
		UNUSED(pData);
		return nullptr;
	}

	// Capabilities Implementation

	void* Renderer::GetMaxTextureSlots_Impl(const void* pData)
	{
		UNUSED(pData);
		return new sint32(s_API->GetMaxTextureSlots());
	}

	void* Renderer::GetMaxTextureSize_Impl(const void* pData)
	{
		UNUSED(pData);
		return new sint32(s_API->GetMaxTextureSize());
	}

	void* Renderer::GetMaxFramebufferWidth_Impl(const void* pData)
	{
		UNUSED(pData);
		return new sint32(s_API->GetMaxFramebufferWidth());
	}

	void* Renderer::GetMaxFramebufferHeight_Impl(const void* pData)
	{
		UNUSED(pData);
		return new sint32(s_API->GetMaxFramebufferHeight());
	}

	void* Renderer::GetMaxFramebufferColorAttachments_Impl(const void* pData)
	{
		UNUSED(pData);
		return new sint32(s_API->GetMaxFramebufferColorAttachments());
	}
}
