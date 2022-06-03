#pragma once

#include "Engine/Core/Timestep.h"

namespace eng
{
	class Input
	{
	private:
		friend class Application;

		static Timestep GetElapsedTime();
		//static void SetEventCallback();
	};
}
