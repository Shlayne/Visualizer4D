#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/CommandLineArgs.h"

namespace eng
{
	class Application
	{
	public:
		Application(CommandLineArgs args);
		virtual ~Application();
		static Application& Get();
	public:
		void Restart();
		void Close();
	private:

	private:
		bool m_Running = false;
	private:
		friend int Main(CommandLineArgs args);
		void Run();
	};
}
