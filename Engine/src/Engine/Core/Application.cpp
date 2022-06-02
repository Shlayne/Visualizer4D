#include "Engine/pch.h"
#include "Application.h"

extern bool g_RestartApplication;

namespace eng
{
	static Application* s_Application = nullptr;

	Application::Application(CommandLineArgs args)
	{
		CORE_ASSERT(!s_Application, "Attempted to recreate Application!");
		s_Application = this;
	}

	Application::~Application()
	{
		CORE_ASSERT(s_Application, "Attempted to redestroy Application!");
		s_Application = nullptr;
	}

	Application& Application::Get()
	{
		return *s_Application;
	}

	void Application::Restart()
	{
		// Just in case g_RestartApplication was reset, set it.
		g_RestartApplication = true;
		m_Running = false;
	}

	void Application::Close()
	{
		g_RestartApplication = false;
		m_Running = false;
	}

	void Application::Run()
	{
		if (m_Running)
			return;

		m_Running = true;

		while (m_Running)
		{

		}
	}
}
