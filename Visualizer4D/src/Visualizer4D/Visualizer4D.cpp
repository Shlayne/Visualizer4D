#include "Visualizer4D/pch.h"
#include <Engine/Core/EntryPoint.h>

namespace eng
{
	Application* CreateApplication(CommandLineArgs args)
	{
		Application* pApplication = new Application(args);

		WindowSpecifications windowSpecs;
		windowSpecs.width = 1600;
		windowSpecs.height = 900;
		windowSpecs.title = "Visualizer 4D";
		UNUSED(pApplication->OpenWindow(windowSpecs));

		return pApplication;
	}
}
