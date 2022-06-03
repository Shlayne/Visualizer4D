#include "Visualizer4D/pch.h"
#include <Engine/Core/EntryPoint.h>

eng::Application* CreateApplication(eng::CommandLineArgs args)
{
	eng::Application* application = new eng::Application(args);

	eng::WindowSpecifications windowSpecs;
	windowSpecs.width = 1600;
	windowSpecs.height = 900;
	windowSpecs.title = "Visualizer 4D";
	application->OpenWindow(windowSpecs);

	return application;
}
