#pragma once

#include "Engine/Rendering/RendererAPI.h"
#include "OpenGLRendererAPI/OpenGLRendererAPIBind.h"
#include <glad/glad.h>

#pragma warning(push)
#pragma warning(disable: 4275)
namespace eng
{
	class OPENGL_RENDERER_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();
	public:
		virtual void EnableDepthTest() override;
		virtual void DisableDepthTest() override;
		virtual void EnableBlending() override;
		virtual void DisableBlending() override;
		virtual void EnableCulling() override;
		virtual void DisableCulling() override;

		virtual void SetViewport(const glm::s32vec2& position, const glm::s32vec2& size) override;

		virtual void Clear() override;
		virtual void ClearDepth() override;
		virtual void SetClearColor(const glm::vec4& color) override;

		//virtual void DrawIndexed() override; // TODO
	public: // Capabilities.
		virtual sint32 GetMaxTextureSlots() override;
		virtual sint32 GetMaxTextureSize() override;
		virtual sint32 GetMaxFramebufferWidth() override;
		virtual sint32 GetMaxFramebufferHeight() override;
		virtual sint32 GetMaxFramebufferColorAttachments() override;
	private:
		GLbitfield m_ClearBits = GL_COLOR_BUFFER_BIT;
	};
}
#pragma warning(pop)
