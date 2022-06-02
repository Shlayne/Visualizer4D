#pragma once

#include "Engine/Core/Core.h"

namespace eng
{
	// In this file, int is explicitly int, not any fancy using or typedef,
	// because that's what's provided by "int main(int argc, char** argv)".

	struct CommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		inline const char* operator[](int index) const
		{
			CORE_ASSERT(index < count, "Command Line Args index={0} out of bounds!", index);
			return args[index];
		}
	};
}
