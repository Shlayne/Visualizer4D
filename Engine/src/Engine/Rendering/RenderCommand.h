#pragma once

#include "Engine/Core/Int.h"

namespace eng
{
	using RenderCommandType = uint8;
	enum : RenderCommandType
	{
		RenderCommand_Shutdown, // Used to exit render thread.

		RenderCommand_EnableDepthTest,
		RenderCommand_DisableDepthTest,
		RenderCommand_EnableBlending,
		RenderCommand_DisableBlending,
		RenderCommand_EnableCulling,
		RenderCommand_DisableCulling,

		RenderCommand_SetViewport,

		RenderCommand_Clear,
		RenderCommand_ClearDepth,
		RenderCommand_SetClearColor,

		RenderCommand_DrawIndexed,

		RenderCommand_GetMaxTextureSlots,
		RenderCommand_GetMaxTextureSize,
		RenderCommand_GetMaxFramebufferWidth,
		RenderCommand_GetMaxFramebufferHeight,
		RenderCommand_GetMaxFramebufferColorAttachments,

		RenderCommand_Count
	};

	class RenderCommand
	{
	public:
		RenderCommand() = default;
		constexpr RenderCommand(RenderCommandType type, void* pData = nullptr)
			: m_Type(type), m_pData(pData) {}
	public:
		constexpr RenderCommandType GetType() const { return m_Type; }
		constexpr void* GetData() const { return m_pData; }
	private:
		RenderCommandType m_Type;
		void* m_pData;
	};
}
