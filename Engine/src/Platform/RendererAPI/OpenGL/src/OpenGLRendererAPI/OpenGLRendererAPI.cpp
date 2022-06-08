#include "OpenGLRendererAPI/OpenGLRendererAPI.h"
#include "OpenGLRendererAPI/OpenGLLogger.h"

namespace eng
{
	static constexpr void DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* cpMessage, const void* cpUserParam)
	{
		UNUSED(source, type, id, length, cpUserParam);
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         LOG_CORE_FATAL(cpMessage); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG_CORE_ERROR(cpMessage); return;
			case GL_DEBUG_SEVERITY_LOW:          LOG_CORE_WARN(cpMessage); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_CORE_TRACE(cpMessage); return;
		}

		CORE_ASSERT(false, "Unknown OpenGL severity level.");
	}

	OpenGLRendererAPI::OpenGLRendererAPI()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if CONFIG_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(&DebugMessageCallback, NULL);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
	}

	void OpenGLRendererAPI::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		m_ClearBits |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
	}

	void OpenGLRendererAPI::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
		m_ClearBits &= ~(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRendererAPI::EnableBlending()
	{
		glEnable(GL_BLEND);
	}

	void OpenGLRendererAPI::DisableBlending()
	{
		glDisable(GL_BLEND);
	}

	void OpenGLRendererAPI::EnableCulling()
	{
		glEnable(GL_CULL_FACE);
	}

	void OpenGLRendererAPI::DisableCulling()
	{
		glDisable(GL_CULL_FACE);
	}

	void OpenGLRendererAPI::SetViewport(const glm::s32vec2& position, const glm::s32vec2& size)
	{
		glViewport(position.x, position.y, size.x, size.y);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(m_ClearBits);
	}

	void OpenGLRendererAPI::ClearDepth()
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	sint32 OpenGLRendererAPI::GetMaxTextureSlots()
	{
		GLint maxTextureSlots = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
		return static_cast<sint32>(maxTextureSlots);
	}

	sint32 OpenGLRendererAPI::GetMaxTextureSize()
	{
		GLint maxTextureSize = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
		return static_cast<sint32>(maxTextureSize);
	}

	sint32 OpenGLRendererAPI::GetMaxFramebufferWidth()
	{
		GLint maxFramebufferWidth = 0;
		glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFramebufferWidth);
		return static_cast<sint32>(maxFramebufferWidth);
	}

	sint32 OpenGLRendererAPI::GetMaxFramebufferHeight()
	{
		GLint maxFramebufferHeight = 0;
		glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFramebufferHeight);
		return static_cast<sint32>(maxFramebufferHeight);
	}

	sint32 OpenGLRendererAPI::GetMaxFramebufferColorAttachments()
	{
		GLint maxFramebufferColorAttachments = 0;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxFramebufferColorAttachments);
		return static_cast<sint32>(maxFramebufferColorAttachments);
	}
}
