#pragma once

#include "Engine/Rendering/Context.h"
#include <glad/glad.h>
#if SYSTEM_WINDOWS
	#define GLFW_INCLUDE_NONE
	#include <glfw/glfw3.h>
#endif
#include "OpenGLRendererAPI/OpenGLRendererAPIBind.h"

#pragma warning(push)
#pragma warning(disable: 4275)
namespace eng
{
	class OPENGL_RENDERER_API OpenGLContext : public Context
	{
	public:
		OpenGLContext(void* pNativeWindow);
	public:
		virtual void SwapBuffers() override;
	public:
		virtual const char* GetVendor() const override;
		virtual const char* GetRenderer() const override;
		virtual const char* GetVersion() const override;
	private:
#if SYSTEM_WINDOWS
		GLFWwindow* m_pWindow;
		mutable float lastTime = 0.0f;
#endif
	};
}
#pragma warning(pop)
