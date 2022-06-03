#include "Engine/pch.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Input.h"

extern bool g_RestartApplication;
extern eng::RendererAPI::API g_NextRendererAPI;

namespace eng
{
	static Application* s_pApplication = nullptr;

	Application::Application(CommandLineArgs args)
	{
		CORE_ASSERT(s_pApplication == nullptr, "Attempted to recreate Application!");
		s_pApplication = this;
	}

	Application::~Application()
	{
		CORE_ASSERT(s_pApplication != nullptr, "Attempted to redestroy Application!");
		s_pApplication = nullptr;
	}

	Application& Application::Get()
	{
		return *s_pApplication;
	}

	void Application::Restart(RendererAPI::API nextRendererAPI)
	{
		g_RestartApplication = true;
		g_NextRendererAPI = nextRendererAPI;
		m_Running = false;
	}

	void Application::Close()
	{
		g_RestartApplication = false;
		g_NextRendererAPI = RendererAPI::GetAPI();
		m_Running = false;
	}

	Ref<Window> Application::OpenWindow(const WindowSpecifications& crWindowSpecs, bool shareContext)
	{
		return m_Windows.emplace_back(Window::CreateRef(crWindowSpecs, shareContext && !m_Windows.empty() ? m_Windows.front() : nullptr));
	}

	Ref<Window> Application::GetWindow(size_t index)
	{
		return index < m_Windows.size() ? m_Windows[index] : nullptr;
	}

	void Application::Run()
	{
		CORE_ASSERT(m_Running == false, "Cannot run the application while it is already running!");
		m_Running = true;

		Timestep timestep;
		while (m_Running)
		{
			timestep = Input::GetElapsedTime();

			if (!m_Windows.empty())
			{
				for (auto& window : m_Windows)
					window->GetContext()->SwapBuffers();
				Window::PollEvents();
			}
		}
	}
}
