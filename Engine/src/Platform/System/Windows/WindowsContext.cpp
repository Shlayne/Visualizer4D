#include "Engine/pch.h"
#include "Engine/Rendering/Context.h"
#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

namespace eng
{
	void WithContext(void* pNativeWindow, const std::function<void()>& crfWork)
	{
		GLFWwindow* pContext = static_cast<GLFWwindow*>(pNativeWindow);

		GLFWwindow* pCurrentContext = glfwGetCurrentContext();
		bool setNewContext = pContext != pCurrentContext;

		if (setNewContext)
			glfwMakeContextCurrent(pContext);
		crfWork();
		if (setNewContext)
			glfwMakeContextCurrent(pCurrentContext);
	}

	void RemoveCurrentContext()
	{
		glfwMakeContextCurrent(NULL);
	}
}
