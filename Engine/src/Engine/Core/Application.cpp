#include "Engine/pch.h"
#include "Engine/Core/Application.h"

extern bool g_RestartApplication;
extern eng::RendererAPI::API g_NextRendererAPI;

namespace eng
{
	static Application* s_pApplication = nullptr;

	Application::Application(CommandLineArgs args)
	{
		UNUSED(args); // TODO

		CORE_ASSERT(s_pApplication == nullptr, "Attempted to recreate Application!");
		s_pApplication = this;

		m_Input = Input::CreateScope(BIND_FUNC(OnEvent));
	}

	Application::~Application()
	{
		m_Windows.clear();
		DestroyScope(m_Input);

		CORE_ASSERT(s_pApplication != nullptr, "Attempted to redestroy Application!");
		s_pApplication = nullptr;
	}

	Application& Application::Get()
	{
		CORE_ASSERT(s_pApplication != nullptr, "Attempted to get the application before it was created!");
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

	Window& Application::OpenWindow(const WindowSpecifications& crWindowSpecs)
	{
		return *m_Windows.emplace_back(m_Windows.empty() ? Window::CreateScope(crWindowSpecs, nullptr) : Window::CreateScope(crWindowSpecs, m_Windows.front()));
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

	size_t Application::GetWindowIndex(const void* cpNativeWindow)
	{
		for (size_t i = 0; i < m_Windows.size(); i++)
			if (m_Windows[i]->GetNativeWindow() == cpNativeWindow)
				return i;
		return m_Windows.size();
	}

	size_t Application::GetWindowIndex(const Window& crWindow)
	{
		return GetWindowIndex(crWindow.GetNativeWindow());
	}

	void Application::OnEvent(Event& rEvent)
	{

	}

	void Application::Run()
	{
		CORE_ASSERT(m_Running == false, "Cannot run the application while it is already running!");
		CORE_ASSERT(!m_Windows.empty(), "Application must have at least one window to run!");
		m_Running = true;

		Input& input = *m_Input;
		while (m_Running)
		{
			Timestep timestep = input.GetElapsedTime();
			input.PollEvents();

			//Update(timestep);

			// Iterate over windows backwards for two reasons.
			// 1) If the first window should be closed, which also means all other windows should be closed,
			//		then close the other windows first.
			// 2) Swapping all other windows before the first one takes the time that the first window
			//		would spend waiting for vsync, i.e. it's faster.
			Window& rFirstWindow = *m_Windows.front();
			for (size_t i = m_Windows.size(); i; )
			{
				auto& rWindow = *m_Windows[--i];
				rWindow.GetContext()->SwapBuffers();
				if (rFirstWindow.ShouldClose() || rWindow.ShouldClose())
					m_Windows.erase(m_Windows.begin() + i);
			}

			if (m_Windows.empty())
				Close();
		}
	}
}
