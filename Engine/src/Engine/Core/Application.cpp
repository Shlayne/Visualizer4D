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

		Input::SetEventCallback(BIND_FUNC(OnEvent));
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

	Window& Application::OpenWindow(const WindowSpecifications& crWindowSpecs, bool shareContext)
	{
		return *m_Windows.emplace_back(Window::CreateRef(crWindowSpecs, shareContext && !m_Windows.empty() ? m_Windows.front() : nullptr));
	}

	Window& Application::GetWindow(size_t index)
	{
		CORE_ASSERT(index < m_Windows.size(), "Window index out of bounds!");
		return *m_Windows[index];
	}

	void Application::CloseWindow(size_t index)
	{
		CORE_ASSERT(index < m_Windows.size(), "Window index out of bounds!");
		m_Windows[index]->Close(); // Mark the window for deletion.
	}

	size_t Application::GetWindowIndex(void* pNativeWindow)
	{
		for (size_t i = 0; i < m_Windows.size(); i++)
			if (m_Windows[i]->GetNativeWindow() == pNativeWindow)
				return i;
		return m_Windows.size();
	}

	void Application::OnEvent(Event& event)
	{

	}

	void Application::Run()
	{
		CORE_ASSERT(m_Running == false, "Cannot run the application while it is already running!");
		m_Running = true;

		Timestep timestep;
		while (m_Running)
		{
			timestep = Input::GetElapsedTime();
			Input::PollEvents();

			if (!m_Windows.empty())
			{
				for (auto& rWindow : m_Windows)
					rWindow->GetContext()->SwapBuffers();

				// Remove closed windows synchronously.
				for (size_t i = 0; i < m_Windows.size(); i++)
				{
					auto& rWindow = m_Windows[i];
					if (rWindow->ShouldClose())
					{
						rWindow.reset(); // Delete the window.
						m_Windows.erase(m_Windows.begin() + i);
					}
				}

				if (m_Windows.empty())
					Close();
			}
		}
	}
}
