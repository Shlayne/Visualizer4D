#pragma once

#if SYSTEM_WINDOWS

#include "Engine/Core/Application.h"
#include "Engine/Core/Logger.h"
#include "Engine/Debug/Profiler.h"

extern eng::Application* CreateApplication(eng::CommandLineArgs args);

bool g_RestartApplication = true;
eng::RendererAPI::API g_NextRendererAPI = eng::RendererAPI::API::None;

namespace eng
{
	int Main(CommandLineArgs args)
	{
		Logger::Init();

		// TODO: change this.
		bool initialSettingRendererAPIStatus = RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		CORE_ASSERT(initialSettingRendererAPIStatus, "Failed to set initial Renderer API!");

		do
		{
			PROFILE_BEGIN_RUNTIME("Init");
			Application* application = CreateApplication(args);
			PROFILE_END_RUNTIME();

			PROFILE_BEGIN_RUNTIME("Run");
			application->Run();
			PROFILE_END_RUNTIME();

			PROFILE_BEGIN_RUNTIME("Shutdown");
			delete application;
			PROFILE_END_RUNTIME();

			if (!RendererAPI::SetAPI(g_NextRendererAPI))
			{
				LOG_CORE_WARN("Could not set Renderer API to {0}. Defaulting to old Renderer API {1}",
					static_cast<uint8>(g_NextRendererAPI),
					static_cast<uint8>(RendererAPI::GetAPI()));
				g_NextRendererAPI = RendererAPI::GetAPI();
			}
		}
		while (g_RestartApplication);

		return 0;
	}
}

#if CONFIG_DIST
#include <Windows.h>

int APIENTRY WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hInstPrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNUSED(hInst, hInstPrev, lpCmdLine, nCmdShow);
	return eng::Main({ __argc, __argv });
}
#else // !CONFIG_DIST
int main(int argc, char** argv)
{
	return eng::Main({ argc, argv });
}
#endif // CONFIG_DIST

#endif // SYSTEM_WINDOWS
