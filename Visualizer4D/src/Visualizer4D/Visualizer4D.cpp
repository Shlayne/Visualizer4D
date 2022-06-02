#include "Visualizer4D/pch.h"
#include <Engine/Core/EntryPoint.h>

eng::Application* CreateApplication(eng::CommandLineArgs args)
{
	return new eng::Application(args);
}
