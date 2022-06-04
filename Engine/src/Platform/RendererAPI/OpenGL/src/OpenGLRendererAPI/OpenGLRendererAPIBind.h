#pragma once

// This should always be included, and be the last include.

#if OPENGL_RENDERER_API_EXPORT
	#define OPENGL_RENDERER_API __declspec(dllexport)
#else
	#define OPENGL_RENDERER_API __declspec(dllimport)
#endif

#if ENABLE_LOGGING
	#include "Engine/Core/Logger.h"
#endif
#if SYSTEM_WINDOWS
	#include <glfw/glfw3.h>
#endif

namespace eng
{
	class OPENGL_RENDERER_API OpenGLRendererAPIBind
	{
	public:
#if ENABLE_LOGGING
		using GetCoreLogger_FUNC = Ref<spdlog::logger>&(*)();
	public:
		static Ref<spdlog::logger>& GetCoreLogger();
	private:
		friend class RendererAPI;
		static void SetLoggerFunctions
		(
			GetCoreLogger_FUNC GetCoreLogger_FPTR
		);
	private:
		static GetCoreLogger_FUNC s_GetCoreLogger;
#endif
#if SYSTEM_WINDOWS
	public:
		using glfwMakeContextCurrent_FUNC = void(*)(GLFWwindow*);
		using glfwGetProcAddress_FUNC = GLFWglproc(*)(const char*);
		using glfwSwapBuffers_FUNC = void(*)(GLFWwindow*);
	public:
		static void glfwMakeContextCurrent(GLFWwindow* pWindow);
		static GLFWglproc glfwGetProcAddress(const char* pProcName);
		static void glfwSwapBuffers(GLFWwindow* pWindow);
	private:
		friend class RendererAPI;
		static void SetGLFWFunctions
		(
			glfwMakeContextCurrent_FUNC glfwMakeContextCurrent_FPTR,
			glfwGetProcAddress_FUNC glfwGetProcAddress_FPTR,
			glfwSwapBuffers_FUNC glfwSwapBuffers_FPTR
		);
	private:
		static glfwMakeContextCurrent_FUNC s_glfwMakeContextCurrent;
		static glfwGetProcAddress_FUNC s_glfwGetProcAddress;
		static glfwSwapBuffers_FUNC s_glfwSwapBuffers;
#endif
	private:
		OpenGLRendererAPIBind() = delete;
		OpenGLRendererAPIBind(const OpenGLRendererAPIBind&) = delete;
		OpenGLRendererAPIBind(OpenGLRendererAPIBind&&) = delete;
		OpenGLRendererAPIBind& operator=(const OpenGLRendererAPIBind&) = delete;
		OpenGLRendererAPIBind& operator=(OpenGLRendererAPIBind&&) = delete;
		~OpenGLRendererAPIBind() = delete;
	};

	// Just to reduce function call code.
	using Bind = OpenGLRendererAPIBind;
}
