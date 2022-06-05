#include "Engine/pch.h"
#include "Platform/System/Windows/WindowsWindow.h"
#include "Engine/Events/WindowEvents.h"
#include "Engine/Events/KeyEvents.h"
#include "Engine/Events/MouseEvents.h"
#include "Engine/Rendering/RendererAPI.h"

// These can only be used once per function, in this order.
#define PUSH_CONTEXT(window) \
		GLFWwindow* prevContext = glfwGetCurrentContext(); \
		glfwMakeContextCurrent(window)
#define POP_CONTEXT() \
		glfwMakeContextCurrent(prevContext)

namespace eng
{
	static uint32 s_OpenWindowCount = 0;

#if ENABLE_ASSERTS
	static GLFWerrorfun s_fPrevErrorCallback = nullptr;

	static void ErrorCallback(int errorCode, const char* pDescription)
	{
		CORE_ASSERT(false, "GLFW Error ({0}): {1}", errorCode, pDescription);
		if (s_fPrevErrorCallback != nullptr)
			s_fPrevErrorCallback(errorCode, pDescription);
	}
#endif

	static Keycode ConvertKeycode(sint32 keycode)
	{
		// TODO: I have made Keycode use the same mappings as glfw,
		// but this *should* technically be a table to index with the keycode.
		return static_cast<Keycode>(keycode);
	}

	static MouseButton ConvertMouseButton(sint32 button)
	{
		// TODO: I have made MouseButton use the same mappings as glfw,
		// but this *should* technically be a table to index with the keycode.
		return static_cast<MouseButton>(button);
	}

	static Modifiers ConvertModifiers(sint32 modifiers)
	{
		return (modifiers & GLFW_MOD_SHIFT     ? Modifiers_Shift    : Modifiers_None) |
			   (modifiers & GLFW_MOD_CONTROL   ? Modifiers_Control  : Modifiers_None) |
			   (modifiers & GLFW_MOD_ALT       ? Modifiers_Alt      : Modifiers_None) |
			   (modifiers & GLFW_MOD_SUPER     ? Modifiers_Super    : Modifiers_None) |
			   (modifiers & GLFW_MOD_CAPS_LOCK ? Modifiers_CapsLock : Modifiers_None) |
			   (modifiers & GLFW_MOD_NUM_LOCK  ? Modifiers_NumLock  : Modifiers_None);
	}

	WindowsWindow::WindowsWindow(const WindowSpecifications& crSpecs, const Ref<Window>& crShareContextWindow)
	{
		if (s_OpenWindowCount == 0)
		{
			PROFILE_SCOPE("glfwInit");

#if ENABLE_ASSERTS
			s_fPrevErrorCallback = glfwSetErrorCallback(&ErrorCallback);
#endif

			int status = glfwInit();
			CORE_ASSERT(status == GLFW_TRUE, "Failed to initialize GLFW!");
		}

		m_State.current.size = { crSpecs.width, crSpecs.height };
		m_State.title = crSpecs.title;
		m_State.resizable = crSpecs.resizable;
		m_State.fullscreen = crSpecs.fullscreen;
		m_State.focused = crSpecs.focusOnShow;

		glfwWindowHint(GLFW_RESIZABLE, m_State.resizable);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, m_State.focused);
#if CONFIG_DEBUG
		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		GLFWmonitor* pPrimaryMonitor = glfwGetPrimaryMonitor();

		if (m_State.fullscreen)
		{
			m_State.preFullscreen.size = m_State.current.size;

			const GLFWvidmode& crVideoMode = *glfwGetVideoMode(pPrimaryMonitor);
			m_State.current.size = { crVideoMode.width, crVideoMode.height };
		}

		{
			PROFILE_SCOPE("glfwCreateWindow");
			m_pWindow = glfwCreateWindow(
				m_State.current.size.x, m_State.current.size.y,
				m_State.title.c_str(),
				m_State.fullscreen ? pPrimaryMonitor : NULL,
				crShareContextWindow ? ((WindowsWindow*)crShareContextWindow.get())->m_pWindow : NULL
			);
		}
		CORE_ASSERT(m_pWindow != NULL, "Failed to create window!");
		s_OpenWindowCount++;

		if (!m_State.fullscreen)
		{
			sint32 left, top, right, bottom;
			glfwGetMonitorWorkarea(pPrimaryMonitor, &left, &top, &right, &bottom);
			glm::s32vec2 monitorWorkArea{ right - left, bottom - top };
			glfwGetWindowFrameSize(m_pWindow, &left, &top, &right, &bottom);
			glm::s32vec2 windowFrameSize{ right + left, bottom + top };

			m_State.current.position = glm::s32vec2{ left, top };
			m_State.current.position += (monitorWorkArea - (m_State.current.size + windowFrameSize)) / 2;

			glfwSetWindowPos(m_pWindow, m_State.current.position.x, m_State.current.position.y);
		}

		glfwSetInputMode(m_pWindow, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
		glfwSetWindowUserPointer(m_pWindow, &m_State);

		m_rContext = Context::CreateRef(m_pWindow);
		SetVsync(crSpecs.vsync);
		SetMouseCapture(crSpecs.mouseCaptured);

		// TODO: always create the window without a monitor.
		// Then maximize it if crSpecs.maximizeOnShow is set.
		// Then set the window's monitor.
		if (crSpecs.maximizeOnShow && !m_State.fullscreen)
		{
			glfwMaximizeWindow(m_pWindow);
			glfwGetWindowPos(m_pWindow, &m_State.current.position.x, &m_State.current.position.y);
			glfwGetWindowSize(m_pWindow, &m_State.current.size.x, &m_State.current.size.y);
			m_State.maximized = true;
		}

		glfwGetFramebufferSize(m_pWindow, &m_State.current.framebufferSize.x, &m_State.current.framebufferSize.y);

		SetCallbacks(m_pWindow);
		glfwShowWindow(m_pWindow);
	}

	WindowsWindow::~WindowsWindow()
	{
		PROFILE_FUNCTION();

		glfwDestroyWindow(m_pWindow);
		if (--s_OpenWindowCount == 0)
		{
#if ENABLE_ASSERTS
			GLFWerrorfun fCallback = glfwSetErrorCallback(s_fPrevErrorCallback);
			CORE_ASSERT(fCallback == &ErrorCallback, "The GLFW error callback was set to {0}, without being reset to {1}, between glfwInit and glfwTerminate!", static_cast<void*>(fCallback), static_cast<void*>(&ErrorCallback));
			s_fPrevErrorCallback = nullptr;
#endif
			glfwTerminate();
		}
	}

	void WindowsWindow::SetTitle(std::string_view title)
	{
		m_State.title = title;
		glfwSetWindowTitle(m_pWindow, title.data());
	}

	void WindowsWindow::SetVsync(bool vsync)
	{
		PUSH_CONTEXT(m_pWindow);
		glfwSwapInterval(vsync ? 1 : 0);
		m_State.vsync = vsync;
		POP_CONTEXT();
	}

	void WindowsWindow::SetResizable(bool resizable)
	{
		glfwSetWindowAttrib(m_pWindow, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
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
			EventCallback(event);
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
			EventCallback(event);
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
			EventCallback(event);
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
			EventCallback(event);
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
			EventCallback(event);
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
			EventCallback(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::DropCallback(GLFWwindow* pWindow, sint32 count, const char** ppPaths)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			WindowPathDropEvent event(pWindow, count, ppPaths);
			EventCallback(event);
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
			EventCallback(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::WindowContentScaleCallback(GLFWwindow* pWindow, float scaleX, float scaleY)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			WindowContentScaleEvent event(pWindow, scaleX, scaleY);
			EventCallback(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::WindowRefreshCallback(GLFWwindow* pWindow)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			WindowRefreshEvent event(pWindow);
			EventCallback(event);
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
					EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyRepeatEvent event(pWindow, ConvertKeycode(keycode), ConvertModifiers(modifiers));
					EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent event(pWindow, ConvertKeycode(keycode), ConvertModifiers(modifiers));
					EventCallback(event);
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
			KeyCharEvent event(pWindow, codepoint);
			EventCallback(event);
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
					EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleaseEvent event(pWindow, ConvertMouseButton(button), ConvertModifiers(modifiers));
					EventCallback(event);
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
			EventCallback(event);
		}

		POP_CONTEXT();
	}

	void WindowsWindow::ScrollCallback(GLFWwindow* pWindow, double offsetX, double offsetY)
	{
		PUSH_CONTEXT(pWindow);

		if (State* pState = static_cast<State*>(glfwGetWindowUserPointer(pWindow)))
		{
			MouseScrollEvent event(pWindow, static_cast<float>(offsetX), static_cast<float>(offsetY));
			EventCallback(event);
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
			EventCallback(event);
		}

		POP_CONTEXT();
	}
}
