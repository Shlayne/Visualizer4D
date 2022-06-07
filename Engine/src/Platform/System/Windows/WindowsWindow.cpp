#include "Engine/pch.h"
#include "Platform/System/Windows/WindowsWindow.h"
#include "Engine/Events/WindowEvents.h"
#include "Engine/Events/KeyEvents.h"
#include "Engine/Events/MouseEvents.h"
#include "Engine/Rendering/RendererAPI.h"
#include "Platform/System/Windows/WindowsConversions.h"

// These can only be used once per function, in this order.
#define PUSH_CONTEXT(window) \
		GLFWwindow* prevContext = glfwGetCurrentContext(); \
		glfwMakeContextCurrent(window)
#define POP_CONTEXT() \
		glfwMakeContextCurrent(prevContext)

namespace eng
{
	WindowsWindow::WindowsWindow(const WindowSpecifications& crSpecs, const Scope<Window>& crShareContextWindow)
	{
		PROFILE_FUNCTION();

		m_State.current.size = { crSpecs.width, crSpecs.height };
		m_State.title = crSpecs.title;
		m_State.resizable = crSpecs.resizable;
		m_State.focused = crSpecs.focusOnShow;

		m_HasSharedContext = static_cast<bool>(crShareContextWindow);

		glfwWindowHint(GLFW_RESIZABLE, m_State.resizable);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, m_State.focused);
		glfwWindowHint(GLFW_VISIBLE, false);
		glfwWindowHint(GLFW_DOUBLEBUFFER, true);
#if CONFIG_DEBUG
		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

		{
			PROFILE_SCOPE("glfwCreateWindow");
			m_pWindow = glfwCreateWindow(
				m_State.current.size.x, m_State.current.size.y,
				m_State.title.c_str(),
				NULL, m_HasSharedContext ? (static_cast<WindowsWindow*>(crShareContextWindow.get()))->m_pWindow : NULL
			);
		}
		CORE_ASSERT(m_pWindow != NULL, "Failed to create window!");

		glm::s32vec2 position{ 0 };
		glm::s32vec2 size{ 0 };
		glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &position.x, &position.y, &size.x, &size.y);
		sint32 left, top, right, bottom;
		glfwGetWindowFrameSize(m_pWindow, &left, &top, &right, &bottom);
		glm::s32vec2 windowFrameSize{ right - left, bottom - (top + 8) };
		// The +8 for top is for the invisible windows 10 borders for resizing the window.
		// Only left, right, and bottom have it, but not top. And since bottom has it and
		// not top, the window is pushed up by 8 / 2 = 4 pixels.

		m_State.current.position = position + (size - m_State.current.size - windowFrameSize) / 2;
		glfwSetWindowPos(m_pWindow, m_State.current.position.x, m_State.current.position.y);

		glfwSetInputMode(m_pWindow, GLFW_LOCK_KEY_MODS, true);
		glfwSetWindowUserPointer(m_pWindow, &m_State);

		{
			PROFILE_SCOPE("Context::CreateScope");
			m_rContext = Context::CreateScope(m_pWindow);
		}
		SetVsync(crSpecs.vsync);
		SetMouseCapture(crSpecs.mouseCaptured);

		if (crSpecs.maximizeOnShow)
		{
			glfwMaximizeWindow(m_pWindow);
			glfwGetWindowPos(m_pWindow, &m_State.current.position.x, &m_State.current.position.y);
			glfwGetWindowSize(m_pWindow, &m_State.current.size.x, &m_State.current.size.y);
			m_State.maximized = true;
		}

		glfwGetFramebufferSize(m_pWindow, &m_State.current.framebufferSize.x, &m_State.current.framebufferSize.y);

		if (crSpecs.fullscreenOnShow)
			SetFullscreen(true);

		SetCallbacks(m_pWindow);
		glfwShowWindow(m_pWindow);
	}

	WindowsWindow::~WindowsWindow()
	{
		PROFILE_FUNCTION();

		glfwDestroyWindow(m_pWindow);
	}

	void WindowsWindow::SetTitle(std::string_view title)
	{
		m_State.title = title;
		glfwSetWindowTitle(m_pWindow, title.data());
	}

	void WindowsWindow::SetVsync(bool vsync)
	{
		if (!m_HasSharedContext)
		{
			PUSH_CONTEXT(m_pWindow);
			glfwSwapInterval(!!vsync);
			m_State.vsync = vsync;
			POP_CONTEXT();
		}
	}

	void WindowsWindow::SetResizable(bool resizable)
	{
		glfwSetWindowAttrib(m_pWindow, GLFW_RESIZABLE, resizable);
		m_State.resizable = resizable;
	}

	void WindowsWindow::SetFullscreen(bool fullscreen)
	{
		GLFWmonitor* pMonitor = NULL;
		sint32 refreshRate = GLFW_DONT_CARE;

		if (fullscreen)
		{
			// From: https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
			// Get the monitor that most of the window is on.
			// On Windows, MonitorFromWindow(window->win32.handle, MONITOR_DEFAULTTONEAREST)
			// does exactly this, but returns the windows monitor handle as opposed to the glfw monitor.
			// However, glfw 3.3 doesn't support using that in this way.

			sint32 largestOverlap = sint32_min;
			const GLFWvidmode* cpVideoMode = NULL;

			sint32 monitorCount;
			GLFWmonitor** ppMonitors = glfwGetMonitors(&monitorCount);

			for (sint32 i = 0; i < monitorCount; i++)
			{
				GLFWmonitor* pCurrentMonitor = ppMonitors[i];
				const GLFWvidmode* cpCurrentVideoMode = glfwGetVideoMode(pCurrentMonitor);
				glm::s32vec2 videoModeSize{ cpCurrentVideoMode->width, cpCurrentVideoMode->height };

				glm::s32vec2 monitorPos;
				glfwGetMonitorPos(pCurrentMonitor, &monitorPos.x, &monitorPos.y);

				glm::s32vec2 overlapSize = glm::max(glm::s32vec2{ 0 },
					glm::min(
						m_State.current.position + m_State.current.size,
						monitorPos + videoModeSize
					) - glm::max(m_State.current.position, monitorPos)
				);

				sint32 overlap = overlapSize.x * overlapSize.y;
				if (overlap > largestOverlap)
				{
					largestOverlap = overlap;
					pMonitor = pCurrentMonitor;
					cpVideoMode = cpCurrentVideoMode;
				}
			}

			CORE_ASSERT(pMonitor != NULL, "Could not find suitable monitor for window to fullscreen into.");
			m_State.preFullscreen = m_State.current;
			m_State.current.position = { 0, 0 };
			m_State.current.size = { cpVideoMode->width, cpVideoMode->height };
			refreshRate = cpVideoMode->refreshRate;
		}
		else
			m_State.current = m_State.preFullscreen;

		glfwSetWindowMonitor(
			m_pWindow, pMonitor,
			m_State.current.position.x, m_State.current.position.y,
			m_State.current.size.x, m_State.current.size.y,
			refreshRate
		);

		glfwGetFramebufferSize(m_pWindow, &m_State.current.framebufferSize.x, &m_State.current.framebufferSize.y);
		SetVsync(IsVsyncEnabled());

		m_State.fullscreen = fullscreen;
	}

	void WindowsWindow::SetMouseCapture(bool mouseCaptured)
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		m_State.mouseCaptured = mouseCaptured;
	}

	bool WindowsWindow::ShouldClose() const
	{
		return m_ShouldClose || glfwWindowShouldClose(m_pWindow);
	}

	void WindowsWindow::SetCallbacks(GLFWwindow* pWindow)
	{
		// Window Events
		glfwSetWindowCloseCallback(pWindow, WindowCloseCallback);
		glfwSetWindowSizeCallback(pWindow, WindowSizeCallback);
		glfwSetWindowPosCallback(pWindow, WindowPosCallback);
		glfwSetWindowFocusCallback(pWindow, WindowFocusCallback);
		glfwSetWindowIconifyCallback(pWindow, WindowIconifyCallback);
		glfwSetWindowMaximizeCallback(pWindow, WindowMaximizeCallback);
		glfwSetDropCallback(pWindow, DropCallback);
		glfwSetFramebufferSizeCallback(pWindow, FramebufferSizeCallback);
		glfwSetWindowContentScaleCallback(pWindow, WindowContentScaleCallback);
		glfwSetWindowRefreshCallback(pWindow, WindowRefreshCallback);

		// Key Events
		glfwSetKeyCallback(pWindow, KeyCallback);
		glfwSetCharCallback(pWindow, CharCallback);

		// Mouse Events
		glfwSetMouseButtonCallback(pWindow, MouseButtonCallback);
		glfwSetCursorPosCallback(pWindow, CursorPosCallback);
		glfwSetScrollCallback(pWindow, ScrollCallback);
		glfwSetCursorEnterCallback(pWindow, CursorEnterCallback);
	}

	void WindowsWindow::WindowCloseCallback(GLFWwindow* pWindow)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			WindowCloseEvent event(pWindow);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::WindowSizeCallback(GLFWwindow* pWindow, sint32 width, sint32 height)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			State& state = *pState;
			state.current.size = { width, height };

			WindowResizeEvent event(pWindow, width, height);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::WindowPosCallback(GLFWwindow* pWindow, sint32 x, sint32 y)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			State& state = *pState;
			state.current.position = { x, y };

			WindowMoveEvent event(pWindow, x, y);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::WindowFocusCallback(GLFWwindow* pWindow, sint32 focused)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			State& state = *pState;
			state.focused = focused == GLFW_TRUE;

			WindowFocusEvent event(pWindow, state.focused);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::WindowIconifyCallback(GLFWwindow* pWindow, sint32 iconified)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			State& state = *pState;
			state.minimized = iconified == GLFW_TRUE;

			WindowMinimizeEvent event(pWindow, state.minimized);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::WindowMaximizeCallback(GLFWwindow* pWindow, sint32 maximized)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			State& state = *pState;
			state.maximized = maximized == GLFW_TRUE;

			WindowMaximizeEvent event(pWindow, state.maximized);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::DropCallback(GLFWwindow* pWindow, sint32 count, const char** ppPaths)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			WindowPathDropEvent event(pWindow, count, ppPaths);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::FramebufferSizeCallback(GLFWwindow* pWindow, sint32 width, sint32 height)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			State& state = *pState;
			state.current.framebufferSize = { width, height };

			WindowFramebufferResizeEvent event(pWindow, width, height);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::WindowContentScaleCallback(GLFWwindow* pWindow, float scaleX, float scaleY)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			WindowContentScaleEvent event(pWindow, scaleX, scaleY);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::WindowRefreshCallback(GLFWwindow* pWindow)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			WindowRefreshEvent event(pWindow);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::KeyCallback(GLFWwindow* pWindow, sint32 keycode, sint32 scancode, sint32 action, sint32 modifiers)
	{
		PUSH_CONTEXT(pWindow);

		State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow));
		if (pState != nullptr && keycode != GLFW_KEY_UNKNOWN)
		{
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressEvent event(pWindow, ConvertKeycode(keycode), ConvertModifiers(modifiers));
					OnEvent(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyRepeatEvent event(pWindow, ConvertKeycode(keycode), ConvertModifiers(modifiers));
					OnEvent(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent event(pWindow, ConvertKeycode(keycode), ConvertModifiers(modifiers));
					OnEvent(event);
					break;
				}
			}
		}

		POP_CONTEXT();
	}

	void WindowsWindow::CharCallback(GLFWwindow* pWindow, uint32 codepoint)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			CharTypeEvent event(pWindow, codepoint);
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::MouseButtonCallback(GLFWwindow* pWindow, sint32 button, sint32 action, sint32 modifiers)
	{
		PUSH_CONTEXT(pWindow);

		State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow));
		if (pState != nullptr)
		{
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressEvent event(pWindow, ConvertMouseButton(button), ConvertModifiers(modifiers));
					OnEvent(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleaseEvent event(pWindow, ConvertMouseButton(button), ConvertModifiers(modifiers));
					OnEvent(event);
					break;
				}
			}
		}

		POP_CONTEXT();
	}

	void WindowsWindow::CursorPosCallback(GLFWwindow* pWindow, double x, double y)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			MouseMoveEvent event(pWindow, static_cast<float>(x), static_cast<float>(y));
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::ScrollCallback(GLFWwindow* pWindow, double offsetX, double offsetY)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			MouseScrollEvent event(pWindow, static_cast<float>(offsetX), static_cast<float>(offsetY));
			OnEvent(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::CursorEnterCallback(GLFWwindow* pWindow, sint32 entered)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			State& state = *pState;
			state.mouseContained = entered == GLFW_TRUE;

			MouseEnterEvent event(pWindow, state.mouseContained);
			OnEvent(event);
		}

		POP_CONTEXT();
	}
}
