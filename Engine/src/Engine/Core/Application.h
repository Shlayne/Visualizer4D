#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/CommandLineArgs.h"
#include "Engine/Core/Window.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/WindowEvents.h"
#include "Engine/Rendering/RendererAPI.h"
#include <vector>

namespace eng
{
	class Application
	{
	public:
		Application(CommandLineArgs args);
		virtual ~Application();
		static Application& Get();
	public:
		void Restart(RendererAPI::API nextRendererAPI = RendererAPI::GetAPI());
		void Close();
	public:
		Window& OpenWindow(const WindowSpecifications& crWindowSpecs);
		Window& GetWindow(size_t index = 0);
		void CloseWindow(size_t index = 0);
		size_t GetWindowIndex(const void* cpNativeWindow);
		size_t GetWindowIndex(const Window& crWindow);
	private:
		void OnEvent(Event& rEvent);
	private:
		std::vector<Ref<Window>> m_Windows;
	private:
		bool m_Running = false;
	private:
		friend int Main(CommandLineArgs args);
		void Run();
	};
}
